#!/usr/bin/env python3


import os
import requests
import sys
# from random import random, shuffle
from time import sleep
import re
import json


def replace(m):
    s:str = m.group(0)
    n:int = ord(s)
    if n < 0x10000:
        return '\\u{0:04x}'.format(n)
        #return '\\u%04x' % (n,)
    else:
        # surrogate pair
        n -= 0x10000
        s1 = 0xd800 | ((n >> 10) & 0x3ff)
        s2 = 0xdc00 | (n & 0x3ff)
        return '\\u{0:04x}\\u{1:04x}'.format(s1, s2)

def json_stresc(s:str):
    # From json.py_encode_basestring_ascii - returns an ASCII-only JSON representation of a Python string
    s = s.replace("\\","\\\\").replace("\n","\\n").replace("\"","\\\"")
    return re.sub('[^\\ -~]', replace, s)

def random():
    RECIP_BPF:float = 2 ** (-53) # Number of bits in a float
    return (int.from_bytes(os.urandom(7), 'big') >> 3) * RECIP_BPF

def randbelow(n:int):
    # from implementation of random._randbelow_with_getrandbits
    k:int = n.bit_length()  # don't use (n-1) here because n can be 1
    r:int = n
    while r >= n:
        r = int.from_bytes(os.urandom((k+7)//8), 'big') # aka getrandbits(k);  0 <= r < 2**k
    return r

def shuffle(x:list):
    for i in reversed(range(1, len(x))):
        # pick an element in x[:i+1] with which to exchange x[i]
        j:int = randbelow(i + 1)
        x[i], x[j] = x[j], x[i]

def send_final_prompt(server_url:str, reiterate_instructions_content:str, f, prefix:str, prompt:str, final_instruction:list, ask_for_n_results:int):
    prompt += final_instruction[0].format(
        count=len(final_instruction[1])+ask_for_n_results,
        plural="s",
        extra_message=reiterate_instructions_content
    )
    j:int = 1
    for answer in final_instruction[1]:
        prompt += str(j)
        prompt += ") "
        prompt += answer
        j += 1
    prompt += str(j)
    prompt += ") "
    if prefix is not None:
        prompt += prefix
    
    n_since_last_flush:int = 0
    for i in range(1):
        temperature:float = 0.68 + 0.04*random()
        repeat_penalty:float = 1.1 + 0.1*random()
        top_p:float = 0.05 + 0.1*random()
        seed:int = int.from_bytes(os.urandom(4), 'big') # A value of -1 means the server chooses a random value each time
        f.write(f"\nprompt_len {len(prompt)}\ntemperature {temperature}\nrepeat_penalty {repeat_penalty}\ntop_p {top_p}\nseed {seed}\n")
        print(f"Requesting {i}")
        try:
                with requests.post(
                    server_url,
                    data=f'''{{"stream": true, "n_predict": 400, "temperature": {temperature}, "stop": ["</s>", "USER:", "ASSISTANT:"], "repeat_last_n": 128, "repeat_penalty": {repeat_penalty}, "top_k": 40, "top_p": {top_p}, "ignore_eos": false, "prompt": "{json_stresc(prompt)}"}}''',
                    headers={
                        'Connection': 'keep-alive',
                        'Content-Type': 'application/json',
                        'Accept': 'text/event-stream'
                    },
                    stream=True
                ) as r:
                    with open("/media/vangelic/DATA/tmp/render-many-single-answers-from-rpill-promptlists.py.last_used_prompt.txt","w") as promptf:
                        promptf.write(prompt)
                    if r.ok:
                        for line_binary in r.iter_lines():
                            if line_binary != b"":
                                d_str:str = line_binary[6:].decode()
                                d:dict = json.loads(d_str)
                                f.write(d["content"])
                                n_since_last_flush += 1
                                if d["stop"]:
                                    f.write("\n\n")
                                elif n_since_last_flush > 4:
                                    f.flush()
                                    n_since_last_flush = 0
                    else:
                        print(f"request failed: {r.text}", file=sys.stderr)
                        sleep(60.0)
        except KeyboardInterrupt:
            raise
        except Exception as e:
            print(f"{e.__class__.__name__}: {e}")
            raise
            sleep(30.0)

def process_to_file(server_url:str, all_prefixes:list, reiterate_instructions_content:str, f, previously_done_entities:list, previously_done_prefixes:list, instructions:str, inputs:list):
    prev_prompt:str = None
    regexp = re.compile("\\{entity=([A-Z_]+),prefix=([A-Za-z ]*)\\}$")
    # TODO maybe: currentprompt_previously_done_prefixes__offset:int = len(previously_done_prefixes)
    while True:
        prompt:str = instructions
        shuffle(inputs)
        i:int = 1
        n_answers:int = 0
        while i != len(inputs):
            n_answers = len(inputs[i][1])
            
            prompt += inputs[i][0].format(
                count=n_answers,
                plural="s" if (n_answers>1) else "",
                extra_message=""
            )
            shuffle(inputs[i][1])
            j:int = 1
            for answer in inputs[i][1]:
                m = regexp.search(answer)
                if m is not None:
                    entity,prefix1 = m.groups()
                    if entity in previously_done_entities:
                        continue
                    previously_done_prefixes.append(prefix1)
                    previously_done_entities.append(entity)
                prompt += str(j)
                prompt += ") "
                prompt += answer
                j += 1
            prompt += "\n"
            
            shuffle(all_prefixes)
            len_all_prefixes:int = len(all_prefixes)
            cntry_i:int = 0
            n_prefixes_done_for_this:int = 0
            send_final_prompt(server_url, reiterate_instructions_content, f, None, prompt, inputs[0], 4)
            while (n_prefixes_done_for_this < 5) and (cntry_i < len_all_prefixes):
                if all_prefixes[cntry_i] not in previously_done_prefixes:
                    send_final_prompt(server_url, reiterate_instructions_content, f, all_prefixes[cntry_i], prompt, inputs[0], 1) # So the final question is asked, preceded by EACH of {1..N-1} of the other input-answer pairs.
                    n_prefixes_done_for_this += 1
                cntry_i += 1
            i += 1



if __name__ == "__main__":
    if len(sys.argv) != 4:
        print("USAGE: [http://example.com/path/to/api] [/path/to/instructions.txt] [REITERATE_INSTRUCTIONS_CONTENT, like 'Don't include any person you previously listed.'] [CONVERSATION_HISTORY_DIRECTORY] [OUTPUT_FILEPATH] [[LIST_OF_PREFIXES_TO_ITERATE_OVER]]", file=sys.stderr)
        exit()
    
    ''' see https://www.reddit.com/r/LocalLLaMA/comments/1343bgz/what_model_parameters_is_everyone_using/
    repeat_last_n works by avoiding using any tokens within the last N tokens:  https://github.com/ggerganov/llama.cpp/issues/787
    top_k Unsure if used in llama.cpp yet: https://github.com/ggerganov/llama.cpp/issues/42
    top_p 0 is greedy inference, picking the highest probability token. Lower numbers more factual?
    '''
    instructions:str = None
    with open(sys.argv[2],"r") as f:
        instructions = f.read().strip()+"\n\n"
    inputs:list = []
    for fname in os.listdir(sys.argv[4]):
        fpath:str = sys.argv[4] + "/" + fname
        if os.path.isfile(fpath):
            ls:list = None
            with open(fpath,"r") as f:
                ls = f.read().strip().split("\n")
            for x in ls[1:]:
                if ")" not in x[:9+4]:
                    raise ValueError(f"Line not start with <integer>): {fpath}")
            inputs.append((ls[0]+"\nASSISTANT:",[x[(x.index(")")+1):].strip()+"\n" for x in ls[1:]]))
    
    outfilepath:str = sys.argv[5]
    if os.path.isfile(outfilepath):
        if os.stat(outfilepath).st_size != 0:
            raise ValueError("File already exists")
    previously_done_entities:list  = []
    previously_done_prefixes:list = []
    with open(outfilepath,"w") as f:
        process_to_file(sys.argv[1], sys.argv[5:], sys.argv[3], f, previously_done_entities, previously_done_prefixes, instructions, inputs)
