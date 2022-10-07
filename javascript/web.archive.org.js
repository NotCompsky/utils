// ==UserScript==
// @name         Localise web.archive.org
// @namespace    http://tampermonkey.net/
// @version      0.1
// @description  try to take over the world!
// @author       You
// @match        https://web.archive.org/web/2*
// @match        https://yandexwebcache.net/*
// @grant        none
// ==/UserScript==

const custombtncontainer = document.createElement("div");
custombtncontainer.id = "custom-btn-container";
document.body.append(custombtncontainer);

const add_css = text => {
    const css = document.createElement("style");
    css.innerText = text;
    document.body.appendChild(css);
};

function tolocalhostcached(href){
	switch(document.location.hostname){
		case "web.archive.org":
			return /^https?:\/\/web[.]archive[.]org\/web\/[0-9_a-z]+\/(.*$)/.exec(href)[1];
		case "yandexwebcache.net":
			return decodeURIComponent(/^https?:\/\/yandexwebcache.net\/yandbtm[?].*&url=([^&]+)&/.exec(href)[1]);
	}
}

function gotourl(eventobj){
	document.location = eventobj.currentTarget.dataset.url;
}

function add_href_btn(text, href){
	const btn = document.createElement("button");
	btn.innerText = text;
	btn.dataset.url = href;
	btn.addEventListener("pointerup", gotourl);
	// btn.setAttribute("href", href);
	btn.classList.add("custom-btn");
	custombtncontainer.append(btn);
	return btn;
}

add_css(`#custom-btn-container {
    position:fixed;
    top:6em;
    left:1em;
    z-index:9999;
}`);

add_href_btn("Localise()", "http://localhost:8080/cached/"+tolocalhostcached(document.location.href));
