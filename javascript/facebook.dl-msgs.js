// ==UserScript==
// @name         Nitter (for Twitter) Buttons
// @namespace    http://tampermonkey.net/
// @version      0.1
// @description  try to take over the world!
// @author       You
// @match        https://nitter.domain.glass/*
// @grant        none
// ==/UserScript==


const set_clipboard = s => navigator.clipboard.writeText(s);

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
};

add_css(`#custom-btn-container {
    position:fixed;
    top:1em;
    left:1em;
    z-index:9999;
}`);


function get_msgs_container_from_box_node(box_node){
	for (let node of box_node.children){
		if (node.ariaLabel && node.ariaLabel.startsWith("Messages in conversation with "))
			return node;
	}
}

function get_fb_msgs_from_box(box_node){
	get_fb_msgs_from_msgs_container(get_msgs_container_from_box_node(box_node));
}

function guess_names(msgs_container, row){
	const xxx = row.getElementsByTagName("span")[0];
	if (xxx === undefined)
		return "???";
	const rect1 = msgs_container.getBoundingClientRect();
	const rect2 = xxx.getBoundingClientRect();
	
	const distance_from_left = rect2.x - rect1.x;
	const distance_from_right = (rect1.x + rect1.width) - (rect2.x + rect2.width);
	
	return (distance_from_left < distance_from_right) ? "OTHER_PERSON" : "Me";
}

function get_fb_msgs_from_msgs_container(msgs_container){
	let tmp;
	let s = "";
	for (let row of msgs_container.children){
		if (row.classList.length === 0){
			// Sets the date after lull in convo
			continue;
		}
		tmp = row.getElementsByTagName("h3")[0];
		const name = (tmp===undefined) ? guess_names(msgs_container, row) : (tmp.innerText.startsWith("You ") ? "Me" : tmp.innerText);
		tmp = row.getElementsByTagName("img")[0];
		const avatar = (tmp===undefined) ? ":::" : tmp.src;
		let content = null;
		let likes = [];
		for (div of row.getElementsByTagName("div")){
			if (div.innerText === "Original message:"){
				// TODO: Identify which message it is replying to
				div.parentNode.remove();
				continue;
			}
			if (div.dataset.testid && div.dataset.testid==="message-container"){
				for (div2 of div.getElementsByTagName("div")){
					if (div2.getAttribute("role")==="button"){
						likes = Array.from(div2.getElementsByTagName("img")).map(x => x.alt);
						div2.remove();
						break;
					}
				}
				Array.from(div.getElementsByTagName("img")).map(x => x.outerHTML = x.alt);
				content = div.innerText;
				continue;
			}
		}
		s += JSON.stringify([name, content, likes]) + ",\n";
	}
	set_clipboard(s);
}
add_btn("Get Msgs from temp0", () => get_fb_msgs_from_msgs_container(temp0));
get_fb_msgs_from_msgs_container(document.getElementsByTagName("div")[0].parentNode.children[2]);
