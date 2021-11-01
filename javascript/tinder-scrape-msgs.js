// ==UserScript==
// @name         Tinder Msg Scraper
// @namespace    http://tampermonkey.net/
// @version      0.1
// @description  
// @author       You
// @match        https://tinder.com/*
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
};

const dl_blob = (name,blob) => {
	const a = document.createElement("a");
	a.href = URL.createObjectURL(blob);
	a.download = name;
	a.hidden = true;
	document.body.appendChild(a);
	a.click();
	document.body.removeChild(a);
};

const dl_text_as_file = (name,text) => dl_blob(name, new Blob([text], {type: "text/plain"}));

/*async*/ function dl_img(name,url){
	/*const r = await fetch(url, {mode:"no-cors", method:"GET", credentials:"include", redirect:"follow"});
	dl_blob(name, await r.blob()); ONLY GETS 0 bytes*/
	
	const a = document.createElement("a");
	a.href = url;
	a.download = name;
	a.hidden = true;
	document.body.appendChild(a);
	a.click();
	document.body.removeChild(a);
};

add_css(`#custom-btn-container {
    position:fixed;
    top:1em;
    left:1em;
    z-index:9999;
	background:grey;
}`);


const msgs_html_prefix = `<!DOCTYPE html>
<html>
<head>
	<style>
.me {
	background:blue;
}
.you {
	background: grey;
}
.msg:hover::after {
	content:attr(data-date);
	color:yellow;
}
.liked::before {
	content:"LIKED";
}
#imgs_container {
	display:flex;
	flex-direction:row;
	overflow-x:scroll;
	height:50vh;
}
	</style>
</head>
<body>
`;
const msgs_html_postfix = `
</body>
</html>`;

const sleepms = ms => new Promise(resolve => setTimeout(resolve, ms));

async function get_msgs(){
	let date = "";
	let msgs = [];
	let her_name = "";
	let matched_date = "";
	for (let node of document.getElementsByTagName("div")){
		if (node.innerText.startsWith("You matched with ")){
			const arr = node.innerText.match(/^You matched with ([^ ]+) on ([0-9]{2}\/[0-9]{2}\/[0-9]{4})/);
			her_name = arr[1];
			matched_date = arr[2];
			break;
		}
	}
	let is_verified = false;
	for (let node of document.getElementsByTagName("title")){
		if (node.innerText.startsWith("Verified!"))
			is_verified = true;
	}
	const chat_container = document.getElementsByClassName("chat")[0].children[2];
	for (let x of chat_container.children){
		let _d = {};
		if (x.classList.contains("msgWrp")){
			const msg_container = x.children[0];
			const t = date + msg_container.children[0].innerText;
			//const t = Math.round(new Date(date + msg_container.children[0].innerText).getTime()/1000);
			let msg = msg_container.children[2];
			if (msg === undefined){
				console.log("msg is undefined for msg_container == ", msg_container);
				continue;
			}
			const is_them = (msg.classList.contains("msg--received"));
			let txt = msg.getElementsByClassName("text")[0];
			if (txt === undefined){
				console.log(msg);
				continue;
			}
			_d["sent_date"] = t;
			_d["message"]   = txt.innerHTML;
			_d["is_them"] = is_them;
			_d["liked"] = (x.getElementsByTagName("path").length===1);
			msgs.push(_d);
		} else if (x.innerText.match("^[012][0-9]/[01][0-9]/20[12][0-9]")){
			date = x.innerText.substr(0,11).replace("/01/"," Jan ").replace("/02/"," Feb ");
			//const _date = x.innerText.split("/");
			//date = _date[2].substr(0,4) + "/" + _date[1] + "/" + _date[0] + " ";
		}
	}
	
	const her_id = her_name + "_" + matched_date.replaceAll("/","-");
	
	const profile_imgs_container = document.getElementsByClassName("profileCard__slider")[0];
	const profile_imgs_btns = profile_imgs_container.parentNode.getElementsByTagName("button");
	const n_profile_imgs = document.getElementsByClassName("profileCard__slider")[0].getElementsByClassName("keen-slider__slide").length;
	let profile_imgs = [];
	for (let i = 0;  i < n_profile_imgs;  ++i){
		profile_imgs_btns[i].click();
		await sleepms(100);
		const url = profile_imgs_container.children[i].getElementsByClassName("profileCard__slider__img")[0].style.backgroundImage.match(/(?:"[^"]*"|^[^"]*$)/)[0].replaceAll('"','');
		const name = url.match(/[^\/]+$/g)[0];
		await dl_img(name, url);
		profile_imgs.push([name,url]);
	}
	dl_text_as_file(
		her_id + ".html",
		msgs_html_prefix + 
		`<h2>${her_name}</h2>
		<time>${matched_date}</time>` +
		'<div id="imgs_container">' +
			profile_imgs.map(x => `<img loading="lazy" src="${x[0]}"/>`).join("") +
		'</div>' +
		`${is_verified?"VERIFIED":""}` +
		msgs.map(x => `<div class="msg ${x.is_them?'you':'me'} ${x.liked?'liked':''}" data-date="${x.sent_date}">\n\t\t${x.message}\n\t</div>`).join("\n\t")
		+ msgs_html_postfix
	);
	alert("wget " + profile_imgs.map(x => x[1]).join(" "));
};

add_btn("Get Msgs", get_msgs);
