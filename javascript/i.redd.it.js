// ==UserScript==
// @name	i.redd.it
// @namespace	https://www.reddit.com/
// @description	Show media items.
// @include	https://www.reddit.com/*
// @version 0.0.1
// @grant    GM_addStyle
// ==/UserScript==

const rootnode = document.querySelector("div[data-adclicklocation=media]");
const all_media_urls = [];
for (let x of rootnode.getElementsByTagName("figure")){
	all_media_urls.push("https://i.redd.it/"+x.children[0].href.match(/https?:\/\/preview[.]redd[.]it\/([a-zA-Z0-9_-]+[.](jpe?g|png|gif|mp4|webp|webm|bmp))($|[?].*)/)[1]);
}
var media_showing_indx = 0;
document.querySelector("a[title=Next]").onclick = function(){
	++media_showing_indx;
	if (media_showing_indx == all_media_urls.length){
		media_showing_indx = 0;
	}
	rootnode.getElementsByTagName("img")[0].src = all_media_urls[media_showing_indx];
};
// Array.from(document.querySelector("div[data-adclicklocation=media]").getElementsByTagName("figure")).map(x => "https://i.redd.it/"+x.children[0].href.match(/https?:\/\/preview[.]redd[.]it\/([a-zA-Z0-9_-]+[.](jpe?g|png|gif|mp4|webp|webm|bmp))($|[?].*)/)[1])
