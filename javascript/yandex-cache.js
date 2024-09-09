// ==UserScript==
// @name         yandex show cache
// @namespace    http://tampermonkey.net/
// @version      2024-09-09
// @description  try to take over the world!
// @author       You
// @match        https://yandex.com/search?text=*
// @grant        none
// ==/UserScript==

for (let node of Array.from(document.getElementsByClassName("Extralinks"))){
    node.outerHTML = '<a href="' + JSON.parse(node.dataset.vnl)["items"][0]["url"] + '">Yandex cache</a>';
}
