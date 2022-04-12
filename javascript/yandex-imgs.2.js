// ==UserScript==
// @name         Yandex Images
// @namespace    http://tampermonkey.net/
// @version      0.1
// @description  try to take over the world!
// @author       You
// @match        https://yandex.com/images/*
// @grant        none
// ==/UserScript==



const add_css = text => {
    const css = document.createElement("style");
    css.innerText = text;
    document.body.appendChild(css);
};

const custombtncontainer = document.createElement("div");
custombtncontainer.id = "custom-btn-container";
document.body.append(custombtncontainer);

const add_btn = (text, fn) => {
    const btn = document.createElement("button");
    btn.innerText = text;
    btn.addEventListener("pointerup", fn);
    btn.classList.add("custom-btn");
    custombtncontainer.append(btn);
    return btn;
};

add_css(`#custom-btn-container {
    position:fixed;
    top:1em;
    left:1em;
    z-index:9999;
}`);


var min_width = 1500;
var min_height = 1500;

const filter = () => {
	for (let meta_node of Array.from(document.getElementsByClassName("serp-item__meta"))){
		const dimensions = meta_node.innerText.match("([0-9]+)×([0-9]+)");
		if (dimensions === null)
			continue;
		if ((parseInt(dimensions[1]) < min_width) || (parseInt(dimensions[2]) < min_height)){
			meta_node.parentNode.parentNode.parentNode.parentNode.style.display = "none";
		}
	}
};

const filter_btn = add_btn("Filter Images", filter);
add_btn("Set W", () => {
	min_width = parseInt(prompt("W"));
	filter_btn.innerText = `Filter Images: ${min_width}x${min_height}`;
});
add_btn("Set H", () => {
	min_height = parseInt(prompt("H"));
	filter_btn.innerText = `Filter Images: ${min_width}x${min_height}`;
});
