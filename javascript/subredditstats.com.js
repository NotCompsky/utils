// ==UserScript==
// @name         Subreddit Stats Tableiser
// @namespace    http://tampermonkey.net/
// @version      0.1
// @description  try to take over the world!
// @author       You
// @match        http://subredditstats.com/subreddit-user-overlaps/*
// @grant        none
// ==/UserScript==

const dl_text_as_file = text => {
	const a = document.createElement("a");
	a.href = URL.createObjectURL(new Blob([text], {type: "text/plain"}));
	a.download = "reddit-cmnts.txt";
	a.hidden = true;
	document.body.appendChild(a);
	a.click();
};

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
add_btn("Get Timestamps", () => {
	const subreddit = window.location.pathname.split("/")[2];
	
	dl_text_as_file(`<h1>Similar subreddits to r/${subreddit} by user overlap</h1>

<p>Type a subreddit name to list its overlaps with other subreddits. The scores listed are "probability multipliers", so a score of 2 means that users of the inputted subreddit are twice as likely to post and comment on that score=2 subreddit. A score of 1 means that users of the inputted subreddit are no more likely to frequent that score=1 subreddit than the average reddit user. A score of 0 means that users of the inputted subreddit never post/comment on that score=0 subreddit.</p>

<table>
	<tr>
		<td>` + Array.from(document.getElementById("outputEl").children).map(x => x.innerText.replace(/ +$/,"").replace("\t","</td>\n\t\t<td>")).join("</td>\n\t</tr>\n\t<tr>\n\t\t<td>") + `</td>
	</tr>
</table>`);
});
