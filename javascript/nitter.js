// ==UserScript==
// @name         Nitter (for Twitter) Buttons
// @namespace    http://tampermonkey.net/
// @version      0.1
// @description  try to take over the world!
// @author       You
// @match        https://nitter.domain.glass/*
// @grant        none
// ==/UserScript==


function dl_text_as_file(name, text){
	const a = document.createElement("a");
	a.href = URL.createObjectURL(new Blob([text], {type: "text/plain"}));
	a.download = name + ".r";
	a.hidden = true;
	document.body.appendChild(a);
	a.click();
}

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


function ancestor_has_class(element, classname) {
	if (element.classList.contains(classname))
		return true;
	return element.parentNode && ancestor_has_class(element.parentNode, classname);
}
function parent_has_class(element, classname) {
	return element.parentNode.classList.contains(classname);
}


function archive_nitter_timeline_item(node){
	let tmp_node;
	
	let retweeted_by = "NULL";
	let video_url = "NULL";
	let video_thumbnail = "NULL";
	let views = "NULL";
	
	if (!node.classList.contains("timeline-item")){
		throw Error("Not a timeline item");
	}
	
	tmp_node = node.getElementsByClassName("retweet-header")[0];
	if (tmp_node !== undefined){
		retweeted_by = tmp_node.innerText.replace(" retweeted","");
	}
	
	const tweet_header = node.getElementsByClassName("tweet-header")[0];
	if (tweet_header === undefined){
		console.log(node);
	}
	const is_main_tweet = parent_has_class(node, "main-tweet");
	
	const avatar_url = tweet_header.getElementsByClassName("avatar").src;
	
	tmp_node = tweet_header.getElementsByClassName("fullname")[0];
	const username = tmp_node.href.match("/[^/]+$")[0].substr(1);
	const fullname = tmp_node.innerText;
	const fullname_ = fullname.replaceAll("\\","\\\\").replaceAll('"','\\"');
	const is_verified = (tmp_node.getElementsByClassName("verified-icon").length!==0);
	const dt = tweet_header.getElementsByClassName("tweet-date")[0].getElementsByTagName("a")[0].title;
	
	const attachments = node.getElementsByClassName("attachments");
	let img_urls = "NULL";
	if (attachments.length !== 0){
		if (attachments[0].getElementsByTagName("video").length){
			const vid = attachments[0].getElementsByTagName("video")[0];
			
			if (vid.classList.contains("gif")){
				video_url = vid.getElementsByTagName("source")[0].src;
			} else {
				video_url = `"https://video.twimg.com/ext_tw_video/${vid.dataset.url.match(/https%3A%2F%2Fvideo.twimg.com%2Fext_tw_video%2F([^.]+[.][a-z0-9]+)/)[1].replaceAll("%2F","/")}"`;
			}
			
			video_thumbnail = vid.poster || "NULL";
		}
		if (attachments[0].getElementsByClassName("still-image").length){
			img_urls = 'list("' + Array.from(attachments[0].getElementsByClassName("still-image")).map(x => x.href.match(/\/pic\/media%2F([A-Za-z0-9+_-]+[.](jpg|png))/)[1].replaceAll("%2F","/")).join("\",\"") + '")';
		}
	}
	
	const tweet_stats = node.getElementsByClassName("tweet-stat");
	const cmnts = tweet_stats[0].innerText.replace(" ","").replaceAll(",","");
	const retweets = tweet_stats[1].innerText.replace(" ","").replaceAll(",","");
	const quotes = tweet_stats[2].innerText.replace(" ","").replaceAll(",","");
	const likes = tweet_stats[3].innerText.replace(" ","").replaceAll(",","");
	
	if (tweet_stats.length === 0){
		console.log(node);
	}
	
	if (tweet_stats.length === 5){
		views = tweet_stats[4].innerText.replace(" ","").replaceAll(",","");
	}
	
	const tweet_content = node.getElementsByClassName("tweet-content")[0].innerText.replaceAll("\\","\\\\").replaceAll('"','\\"');
	
	const tweet_id = node.getElementsByClassName("tweet-date")[0].getElementsByTagName("a")[0].href.match("/([0-9]+)(#.*)?$")[1];
	
	const profile_img_url_base = node.getElementsByClassName("avatar")[0].src;
	const profile_img_match = profile_img_url_base.match(/\/pic\/profile_images%2F([0-9A-Za-z%_-]+[.](jpe?g|png|gif|webp))$/);
	let profile_img = profile_img_url_base;
	if (profile_img_match===null){
		console.log("profile_img doesn't fit regex", profile_img);
	} else
		profile_img = profile_img_match[1].replace("%2F","/");
	
	return `tweet(
	id="${tweet_id}",
	username="${username}",
	name="${fullname_}",
	dt="${dt}",
	likes=${likes},
	retweets=${retweets},
	quotes=${quotes},
	views=${views},
	is_main_tweet=${is_main_tweet?"TRUE":"FALSE"},
	profile_img="${profile_img}",
	verified=${is_verified?"TRUE":"FALSE"},
	imgs=${img_urls},
	vid=${video_url},
	text="${tweet_content}"
)`;
}

add_btn("Get", ()=>{
	const name = prompt("Name");
	dl_text_as_file(name, 'name <- twitter(list(' + Array.from(document.getElementsByClassName("timeline-item")).filter(x => !(x.classList.contains("more-replies")||x.classList.contains("unavailable")||x.classList.contains("show-more"))).map(x => archive_nitter_timeline_item(x)).join(", ") + "))");
});
