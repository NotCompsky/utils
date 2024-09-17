// ==UserScript==
// @name         Suno, download button
// @namespace    http://tampermonkey.net/
// @version      2024-09-15
// @description  Adds download button to suno.ai
// @author       You
// @match        https://suno.com/*
// @grant        none
// ==/UserScript==

for (let node of Array.from(document.getElementsByTagName("a"))){
	const m = node.href.match(/\/song\/([a-f0-9]{8}-[a-f0-9]{4}-[a-f0-9]{4}-[a-f0-9]{4}-[a-f0-9]{12})$/);
	if (m !== null){
		const new_node = document.createElement("a");
		new_node.href = "https://cdn1.suno.ai/" + m[1] + ".mp3";
		new_node.download = m[1];
		new_node.innerText = "Download";
	}
}