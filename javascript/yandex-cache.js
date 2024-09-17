// ==UserScript==
// @name         yandex show cache
// @namespace    http://tampermonkey.net/
// @version      2024-09-09
// @description  try to take over the world!
// @author       You
// @match        https://yandex.com/search?text=*
// @grant        none
// ==/UserScript==

function yandexcacheurl2url(yandexcache_url){
    const searchforstr = 'https://yandexwebcache.net/yandbtm?';
	const yandex_cache_url_begin_index = yandexcache_url.indexOf(searchforstr);
	const url_within_this = yandexcache_url.substr(yandex_cache_url_begin_index);
    
    let url_start = url_within_this.indexOf("&url=");
	if (url_start !== -1){
		url_start += 5;
		let url_enddd = url_within_this.indexOf("&",url_start);
		if (url_enddd === -1)
			url_enddd = url_within_this.length;
        return decodeURIComponent(url_within_this.substr(url_start, url_enddd-url_start));
    }
}

for (let node of Array.from(document.getElementsByClassName("Extralinks"))){
    for (let [key,val] of Object.entries(node.dataset)){
        if (val.startsWith("{")){
            const yandexcache_url = JSON.parse(val)["items"][0]["url"];
            node.outerHTML = '<a href="' + yandexcache_url + '">Yandex cache</a> <a href="http://localhost:8080/cached/'+yandexcacheurl2url(yandexcache_url)+'">CACHED</a>';
        }
    }
}