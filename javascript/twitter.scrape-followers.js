var prev_node = null;
var root_node = null;

function init(){
	for (let node of document.getElementsByTagName("div")){
		if (node.getAttribute("aria-label") === "Timeline: Following"){
			root_node = node.childNodes[0];
			break;
		}
	}
}

function onscroll(){
	let usernames = "";
	for (let node of root_node.childNodes){
		/*const actual_node = node.childNodes[0].childNodes[0].childNodes[0];
		const r_col = actual_node.childNodes[1];*/
		const username_node = node.getElementsByTagName("span")[2];
		if (username_node === undefined){
			continue;
		username_node.innerText; // TODO
	}
}
