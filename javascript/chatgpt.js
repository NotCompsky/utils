// ==UserScript==
// @name         ChatGPT
// @namespace    http://tampermonkey.net/
// @version      0.1
// @description  try to take over the world!
// @author       You
// @match        https://chatgpt.com/c/*
// @grant        none
// ==/UserScript==

var saved_string = "";
var n_since_logged = 0;
var prev_logged_string = "";
var fn_id = 0;
function mainfn(){
	const s = window.getSelection().toString();
	if (s){
		if (!s.includes("This content may violate our")){
			if (++n_since_logged > 10){
				if (s !== saved_string){
					saved_string = s;
					if (saved_string !== prev_logged_string){
						prev_logged_string = saved_string;
					}
				}
				n_since_logged = 0;
			}
		}
	}
}


const add_intervalfn_btn = document.createElement("button");
add_intervalfn_btn.innerText = "Start timer";
add_intervalfn_btn.addEventListener("pointerup", ()=>{
	if (fn_id !== 0){
		clearInterval(fn_id);
		add_intervalfn_btn.innerText = "Start timer";
	} else {
		fn_id = setInterval(mainfn, 400);
		add_intervalfn_btn.innerText = "Stop timer";
	}
});
document.body.appendChild(add_intervalfn_btn);

const get_savedstring_btn = document.createElement("button");
get_savedstring_btn.innerText = "Get text";
get_savedstring_btn.addEventListener("pointerup", ()=>{
	navigator.clipboard.writeText(saved_string);
});
document.body.appendChild(get_savedstring_btn);
