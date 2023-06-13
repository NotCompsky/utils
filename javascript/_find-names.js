// ==UserScript==
// @name         Highlight Names in localhost
// @namespace    http://tampermonkey.net/
// @version      0.1
// @description  try to take over the world!
// @author       You
// @match        http://localhost:8080/cached/*
// @grant        none
// ==/UserScript==

var name1 = "(Mc)?[A-Z][a-z]+";
var name = `(${name1}|\\[${name1}\\])`;
var org_name_word = `(${name1}|of|the|in|for)`;
var r = new RegExp(`${name}([ -](${org_name_word}|\\[${org_name_word}\\]))* ${name}`, "g");
var anti_r = new RegExp("^(That|Then|Support|Now|In) the");
var ignore_these_terms = [
	"Soviet Russia",
	"Soviet Union",
	"Vietnam War",
	"World War",
	"Cold War",
	"United States",
	"Western Europe",
	"Eastern Europe",
	"Southern Europe",
	"Northern Europe",
	"Third World",
	"New York",
	"New York City",
	"Freedom of Information",
	"The Best",
	"Israeli Zionist",
	"North Korea",
	"Democratic Republic of Congo",
];
function repl_fn(matched_str, offset, full_input_str){
	if (anti_r.exec(matched_str) !== null)
		return matched_str;
	let classname = "customhl hl_name";
	if (ignore_these_terms.includes(matched_str))
		classname = "customhl hl_name_maybe";
	return `<b class="${classname}">${matched_str}</b>`;
}
function hl_names(){
	for (let node of Array.from(document.getElementsByClassName("customhl"))){
		node.outerHTML = node.innerHTML;
	}
	for (let p_node of Array.from(document.getElementsByTagName("p"))){
		const old_html = p_node.innerHTML;
		const new_html = old_html.replaceAll(r, repl_fn);
		if (new_html !== old_html)
			p_node.innerHTML = new_html;
	}
}
hl_names();
