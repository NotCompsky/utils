#include <stdio.h>  // for printf
#include <stdlib.h> // for malloc
#include <string.h> // for memcpy

#include "bool.h"


bool contains_substring(const char* buf,  const char* s,  const size_t s_len){
	size_t n_matched_chars = 0;
	while(*buf != 0){
		if (*buf == *s){
			++s;
			++n_matched_chars;
			if (n_matched_chars == s_len)
				return true;
		} else {
			n_matched_chars = 0;
		}
		++buf;
	}
	return false;
}


void echo_if_contains(char* const buf,  const size_t s_len,  const char* const s,  const char* const fp){
	FILE* const f = fopen(fp, "rb");
	if (!f){
		fprintf(stderr, "Cannot open file: %s\n", fp);
		return;
	}
	
	size_t bytes_read;
	fread(buf,  1,  s_len,  f);
	do {
		bytes_read = fread(buf + s_len,  1,  s_len,  f);
		buf[s_len + bytes_read] = 0;
		if (contains_substring(buf, s, s_len)){
			printf("%s\n", fp);
			return;
		}
		memcpy(buf,  buf + s_len,  s_len);
	} while (bytes_read == s_len);
}


int main(const int argc,  const char** const argv){
	const char* const s = argv[1];
	const size_t s_len = strlen(s);
	
	char* const buf = (char*)malloc(s_len * 2 + 1);
	
	for (auto i = 2;  i < argc;  ++i)
		echo_if_contains(buf, s_len, s, argv[i]);
}
