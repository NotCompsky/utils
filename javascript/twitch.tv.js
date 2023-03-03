// ==UserScript==
// @name	twitch.tv athene
// @namespace	https://www.twitch.tv/
// @description	Download media items.
// @include	https://www.twitch.tv/atheneaiheroes
// @version 0.0.1
// @grant    GM_addStyle
// ==/UserScript==

function fnnn(){
	for (let node of Array.from(document.getElementsByClassName("chat-line__message-container"))){
		const textnode = node.getElementsByClassName("text-fragment")[0];
		let hidethis = true;
		if (textnode !== undefined){
			hidethis = !textnode.innerText.toLowerCase().match(/^(gandalf|saruman|gollum|frodo|sam|elrond|aragorn|galadriel|legolas|boromir|sauron|gimli|pippin|merry|xqc|greta|asmongold|pokelawls|amouranth|hasanabi|coffeezilla|steve jobs|jordan peterson|pewdiepie|joe biden)/);
		}
		if (hidethis)
			node.style.display="none";
	}
}

var ems = 10;

function create_btn(text, fn){
	const add_btns_btn = document.createElement("button");
	add_btns_btn.addEventListener("click", fn);
	add_btns_btn.innerText = text;
	return add_btns_btn;
}

function place_btn(btn_container){
	btn_container.classList.add("my-geturls-btn-container");
	btn_container.style.top = ems + "em";
	ems += 2;
	document.body.appendChild(btn_container);
}

function add_css(text){
	const css = document.createElement("style");
	css.innerText = text;
	document.body.appendChild(css);
}

add_css(`
.my-geturls-btn-container button {
	position:absolute;
	right:0;
	top:0;
	z-index:999;
	min-width:20em;
	min-height:2em;
}
.my-geturls-btn-container {
	position:fixed;
	right:0;
	z-index:9999999;
	min-width:20em;
	min-height:2em;
	background:#f00;
}`);

const btn = create_btn("Hide Non-Qs", fnnn);
const div = document.createElement("div");
div.classList.add("my-geturls-btn-container");
div.appendChild(btn);
place_btn(div);
