// ==UserScript==
// @name	tagem-archive-reddit-posts
// @namespace	https://goooooooooooooooooooooooooooooooooooooooooooooooooooooooogle.com
// @description	Adds buttons to new.reddit.com (eugh!) that will tell the tagem server to archive the submissions
// @include	https://new.reddit.com/*
// @require https://ajax.googleapis.com/ajax/libs/jquery/3.4.1/jquery.min.js 
// @version 0.0.1
// ==/UserScript==

const tagem_server = "localhost:1999";
const tagem_archive_post_btn_class = "tagem-archive-post-btn";
var btn;
var post;

function add_post_to_db(){
  console.log("add_post_to_db()");
  
	//let score    = this.parentNode.childNodes[0].childNodes[0].childNodes[1].innerText;
	
	//let username = this.parentNode.childNodes[1].getElementsByTagName('div')[2].getElementsByTagName('div')[0].getElementsByTagName('a')[0].innerText;
	//let title    = this.parentNode.childNodes[1].childNodes[0].childNodes[0].childNodes[1].childNodes[1].childNodes[0].childNodes[0].childNodes[0].innerText;
	//let link     = this.parentNode.childNodes[1].childNodes[0].childNodes[0].childNodes[2].childNodes[0].href;
	
  let ls = this.parentNode.childNodes[1].childNodes;
	let permalinkk = ls[ls.length - 1].childNodes[1].childNodes[0].href;
	//let n_cmnts   = this.parentNode.childNodes[1].childNodes[1].childNodes[1].childNodes[0].childNodes[1].innerText;
	
  console.log("add_post_to_db() permalink ==", permalinkk);
  
	$.ajax({
		method: "POST",
		url:"http://" + tagem_server + "/x/dl/reddit/p/" + permalinkk,
		success: function(){ alert("Success")},
		error:   function(){ alert("Error")}
	});
  
  console.log("add_post_to_db() finished");
}

function paste_btns(){
  console.log("paste_btns()");
  let post_containers = document.evaluate('/html/body/div[1]/div/div[2]/div[2]/div/div/div/div[2]/div[3]/div[1]/div[5]/div', document, null, XPathResult.ANY_TYPE, null);
	let posts = [];
  while(true){
		post = post_containers.iterateNext();
		if(post === null)
			break;
		posts.push(post.getElementsByTagName('div')[0].getElementsByTagName('div')[0]);
	}
	for(post of posts){
    if(post === undefined)
      // Inline advert?
      continue;
    if(post.getElementsByClassName(tagem_archive_post_btn_class).length !== 0)
      continue;
    
		btn = document.createElement("button");
    btn.classList.add(tagem_archive_post_btn_class);
		btn.innerText = "Record in DB";
		btn.onclick = add_post_to_db;
		post.appendChild(btn);
	}
  
	btn = document.createElement("button");
    btn.classList.add("libya");
		btn.innerText = "Add Buttons Again";
		btn.onclick = paste_btns;
    btn.style.position = "fixed";
    btn.style.background = "#409";
    btn.style.top = "100px";
    btn.style.left = "10px";
    btn.style.zIndex = "100";
		document.getElementsByTagName('body')[0].appendChild(btn);
}

window.addEventListener('load', paste_btns, false);
