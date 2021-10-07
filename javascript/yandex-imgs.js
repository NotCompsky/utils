function dl_text_as_file(text){
	const a = document.createElement("a");
	a.href = URL.createObjectURL(new Blob([text], {type: "text/plain"}));
	a.download = "a.html";
	a.hidden = true;
	document.body.appendChild(a);
	a.click();
}

(function(){
	let s = `
<head>
<style>
#urls-ls {
	whitespace:pre;
	height:15em;
	overflow-y:scroll;
}
#entry-container {
	display: flex;
	flex-wrap:wrap;
}
.entry {
	background:#ccc;
	padding:3px;
	margin:1em;
}
.added {
	background:#ccf;
}
</style>
</head>
<body>
<script>
let all_urls = [];
function add_url(node){
	const url = node.dataset.url;
	if (all_urls.includes(url)){
		all_urls = all_urls.filter(x => x!=url);
		node.parentNode.classList.remove("added");
	} else {
		all_urls.push(url);
		node.parentNode.classList.add("added");
	}
	document.getElementById("urls-ls").innerText = all_urls.join("\\n");
}
</script>
<h1>Yandex Images</h1>
<button onclick="Array.from(document.getElementsByClassName('added')).map(x => x.remove()); all_urls=[]">Remove added</button>
<div id="urls-ls"></div>
<div id="entry-container">`;
	for (let node of document.getElementsByClassName("serp-item__link")){
		const img_url = decodeURIComponent(node.href.match(/&img_url=([^&]+)/)[1]);
		const thumb = node.getElementsByClassName("serp-item__thumb")[0];
		const preview_img_url = thumb.src;
		const descr = thumb.alt;
		const WxH = node.getElementsByClassName("serp-item__hd")[0].innerText;
		s += `
	<div class="entry">
		<a data-url="${img_url}" onclick="add_url(this)">
			<img src="${preview_img_url}"/>
		</a>
	</div>`;
	}
	s += `
</div>
</body>
`;
	dl_text_as_file(s);
}());
