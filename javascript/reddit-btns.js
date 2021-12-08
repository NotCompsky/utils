// ==UserScript==
// @name         Reddit Buttons
// @namespace    http://tampermonkey.net/
// @version      0.1
// @description  try to take over the world!
// @author       You
// @match        https://www.reddit.com/*
// @grant        none
// ==/UserScript==

const get_timestamp_ = x => Date.parse((x.dateTime===undefined) ? x.datetime : x.dateTime)/1000;

const get_all_timestamps = () => Array.from(document.getElementsByClassName("live-timestamp")).map(get_timestamp_).join(",");

const dl_text_as_file = text => {
	const a = document.createElement("a");
	a.href = URL.createObjectURL(new Blob([text], {type: "text/plain"}));
	a.download = "reddit-cmnts.txt";
	a.hidden = true;
	document.body.appendChild(a);
	a.click();
};

const reddit36decode = str => {
	let n = 0;
	for (char of str.split("")){
		n *= 36;
		n += ['0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'].indexOf(char);
	}
	return n;
};

const iterate_over_all_things = fn => Array.from(document.getElementsByClassName("thing")).map(x => fn(x));

function reddit_cmnts(as_sql){
	/*let div = document.getElementById("reddit-cmnts-div");
	if (!div){
		div = document.createElement("div");
		div.style.position = "absolute";
		div.style.top = "0";
		div.style.left = "0";
		div.style.zIndex = "9999";
		div.style.background = "#000";
		div.style.whiteSpace = "pre";
		div.id = "reddit-cmnts-div";
		document.body.appendChild(div);
	}
	
	
	
	div.innerText = */
	
	let submissions_txt = "";
	dl_text_as_file((as_sql?"INSERT INTO cmnt (id,post,user,subreddit,t,score,text) VALUES\n":"") + iterate_over_all_things(x => {
		const parent_container = x.getElementsByClassName("parent")[0];
		let subreddit_div = parent_container.getElementsByClassName("subreddit")[0];
		
		let subreddit;
		const parent_author_div = parent_container.getElementsByTagName("span")[0];
		const parent_author = (parent_author_div === undefined) ? "???" : parent_author_div.innerText;
		
		const child_container = x.getElementsByClassName("entry")[0];
		
		const user_div = child_container.getElementsByClassName("author")[0];
		const username = user_div.innerText;
		
		const livetimestamp = child_container.getElementsByClassName("live-timestamp")[0] || child_container.getElementsByTagName("time")[0];
		const t = (livetimestamp===undefined) ? "???" : get_timestamp_(livetimestamp);
		
		const midcol = x.getElementsByClassName("midcol")[0];
		let score;
		
		if (subreddit_div === undefined){
			// Is a post by the user himself
			subreddit_div = child_container.getElementsByClassName("subreddit")[0];
			subreddit = (subreddit_div === undefined) ? "???" : subreddit_div.innerText;
			score = (midcol.getElementsByClassName("score").length) ? midcol.getElementsByClassName("score")[1].innerText : "NULL";
			const linked_url = x.getElementsByClassName("title")[0].href;
			const text = x.getElementsByClassName("title")[0].innerText;
			const permalink = x.getElementsByClassName("comments")[0].href;
			const post_id = permalink.split("/")[6];
			if (as_sql)
				submissions_txt += `(${post_id}, (SELECT id FROM user WHERE name="${username}"), (SELECT id FROM subreddit WHERE name="${subreddit}"), ${t}, ${score}, "${linked_url}", "${text}"), -- ${permalink}\n`;
			else
				submissions_txt += `Post by /u/${username} in ${subreddit}`;
		} else {
			const permalink = x.getElementsByClassName("flat-list buttons")[0].getElementsByClassName("bylink")[0].href;
			const cmnt_id_str = permalink.split("/")[8];
			const cmnt_id = reddit36decode(cmnt_id_str);
			
			const post_id_str = permalink.split("/")[6];
			const post_id = reddit36decode(post_id_str);
			
			// Array.from(x.getElementsByTagName("blockquote")).map(x => x.innerText = '<BLOCKQUOTE>'+x.innerText+'</BLOCKQUOTE>');
			
			const text = x.getElementsByClassName("usertext-body")[0].innerText.replaceAll("\\","\\\\").replaceAll('"','\\"').replaceAll("\n","\\n");
			
			subreddit = subreddit_div.innerText;
			
			const is_original_poster = user_div.classList.contains("submitter");
			
			score = (child_container.getElementsByClassName("score").length) ? child_container.getElementsByClassName("score")[1].innerText.split(" ")[0] : "NULL";
			
			if (as_sql)
				return `(${cmnt_id}, ${post_id}, (SELECT id FROM user WHERE name="${username}"), (SELECT id FROM subreddit WHERE name="${subreddit}"), ${t}, ${score}, "${text}"), -- ${permalink}`;
			
			return `reddit_cmnt(
	id="${cmnt_id_str}",
	t=${t},
	username="${username}",
	subreddit="${subreddit}",
	original_poster=${is_original_poster ? "TRUE" : "FALSE"},
	text="${text}",
	score=${score},
	post_id="${post_id}"
	# permalink = ${permalink}
)`;
	
			return `In response to a post in /r/${subreddit} by /u/${parent_author}:\n  /u/${username} wrote at ${t}`;
		}
	}).join("\n") + ";\n\n\nINSERT INTO post (id,user,subreddit,t,score,linked_url,text) VALUES\n" + submissions_txt);
}



const empty_cmnt = cmnt => {
	edit_usertext(cmnt.getElementsByClassName("edit-usertext")[0]);
	cmnt.getElementsByClassName("usertext-edit")[0].getElementsByTagName("textarea")[0].innerHTML = "";
	cmnt.getElementsByClassName("usertext-buttons")[0].getElementsByClassName("save")[0].click();
};

iterate_over_all_things(x => {
	const empty_cmnt__btn = document.createElement("button");
	empty_cmnt__btn.innerText = "EMPTY";
	empty_cmnt__btn.classList.add("empty-cmnt-btn");
	empty_cmnt__btn.addEventListener("click", ()=> {
		empty_cmnt(x);
	});
	x.appendChild(empty_cmnt__btn);
});


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
add_btn("Get Timestamps", () => set_clipboard(get_all_timestamps()));

add_btn("reddit_cmnts as redpill", () => reddit_cmnts(false));
add_btn("reddit_cmnts as SQL", () => reddit_cmnts(true));

add_btn("Gallery imgs", () => {
	alert(Array.from(document.getElementsByClassName("media-gallery")[0].getElementsByClassName("gallery-preview")).map(x => x.getElementsByClassName("media-preview-content")[0].getElementsByTagName("a")[0].href.replace("preview.redd.it","i.redd.it").replace(/[?].*/,"")).join("\n"));
});

const entry2t = thing => {
	const livetimestamp = thing.getElementsByClassName("live-timestamp")[0] || thing.getElementsByTagName("time")[0];
	return (livetimestamp===undefined) ? "???" : get_timestamp_(livetimestamp);
};

add_btn("RM /r/???", () => {
	const subreddit_to_rm = prompt("/r/");
	iterate_over_all_things(x => {
		const parent_container = x.getElementsByClassName("parent")[0];
		let subreddit_div = parent_container.getElementsByClassName("subreddit")[0];
		
		let subreddit;
		const parent_author_div = parent_container.getElementsByTagName("span")[0];
		const parent_author = (parent_author_div === undefined) ? "???" : parent_author_div.innerText;
		
		const child_container = x.getElementsByClassName("entry")[0];
		
		const user_div = child_container.getElementsByClassName("author")[0];
		const username = user_div.innerText;
		
		const t = entry2t(child_container);
		
		const midcol = x.getElementsByClassName("midcol")[0];
		let score;
		
		if (subreddit_div === undefined){
			// Is a post by the user himself
			subreddit_div = child_container.getElementsByClassName("subreddit")[0];
			subreddit = (subreddit_div === undefined) ? "???" : subreddit_div.innerText;
			
		} else {
			subreddit = subreddit_div.innerText;
		}
		
		if (subreddit === subreddit_to_rm)
			x.remove();
	});
});





add_btn("RM timestamp<=?", () => {
	const max_t = parseInt(prompt("Max (not inclusive)"));
	iterate_over_all_things(x => {
		if (entry2t(x) <= max_t)
			x.remove();
	});
});

add_btn("Get all text", () => {
	let text = "";
	iterate_over_all_things(x => {
		text += '\n' + x.getElementsByClassName("usertext-body")[0].innerText;
	});
	set_clipboard(text);
});
