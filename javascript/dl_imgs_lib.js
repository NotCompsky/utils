const dl_imgs_lib = function(named_args){


const thread_css_selector = named_args["thread_css_selector"];
const extra_css = named_args["extra_css"] || "";
const btn_pressed__getnode = named_args["btn_pressed__getnode"];
const add_addbtns_btn = named_args["add_addbtns_btn"];
const get_url_from_selected = named_args["get_url_from_selected"];
const hide_thread = named_args["hide_thread"];


const group_colours = ["#00f", "#0c0", "#c00", "#880", "#0cc"];
const max_groups = group_colours.length;
let group_indx2instances = group_colours.map(x => 0);
let group_indx2is_initialised = [];
var ems = 0;

function get_all_selected_nodes(e){
	const n = e.target.dataset.n;
	let ls = Array.from(document.getElementsByClassName("my-selected"+n));
	if ((ls.length === 0) && (n === '0')){
		ls = Array.from(document.getElementsByClassName("my-selection-btn")).map(x => btn_pressed__getnode(x));
	}
	return ls;
}

function create_btn(text, fn){
	const add_btns_btn = document.createElement("button");
	add_btns_btn.addEventListener("click", fn);
	add_btns_btn.innerText = text;
	return add_btns_btn;
}

function place_btn(btn_container){
	btn_container.classList.add("my-geturls-btn-container");
	btn_container.style.top = ems + "em";
	ems += 2;
	document.body.appendChild(btn_container);
}

function add_group_btns(fn, n){
	const new_node = document.createElement("div");
	
	const this_create_btn = (text, wot_fn) => {
		const add_btns_btn = create_btn(text, wot_fn);
		add_btns_btn.dataset.n = n;
		new_node.appendChild(add_btns_btn);
	};
	
	new_node.classList.add("my-selected"+n+"-btn");
	this_create_btn("Download", fn);
	this_create_btn("Rev-Srch", e => {
		alert("firefox " + get_all_selected_nodes(e).map(x => `https://yandex.com/images/search?text=&rpt=imageview&url=${get_url_from_selected(x)}`).join(" "));
	});
	
	const title = document.createElement("span");
	title.innerText = "titlehere";
	title.contentEditable = true;
	new_node.appendChild(title);
	
	place_btn(new_node);
}

function get_next_nonempty_group_indx(n){
	return Array.from(group_indx2instances.entries()).filter(([indx,value]) => value).filter(([indx,value]) => (indx>n))[0][0];
}

function set_node_new_n(node, new_n){
	if (node.dataset.n !== undefined){
		node.classList.remove("my-selected"+node.dataset.n);
		group_indx2instances[node.dataset.n] -= 1;
	}
	node.dataset.n = new_n;
	if (new_n !== undefined){
		node.classList.add("my-selected"+new_n);
		group_indx2instances[new_n] += 1;
	}
}

function add_css(text){
	const css = document.createElement("style");
	css.innerText = text;
	document.body.appendChild(css);
}

function selected_css_rules(i){
	add_css(`.my-selected${i}, .my-selected${i}-btn {
		background: ${group_colours[i]};
		outline:4px ${group_colours[i]} solid;
	}`);
}

function get_all_selected(e){
	const n = e.target.dataset.n;
	alert(get_all_selected_nodes(e).map(x => {
		x.classList.add("hidden");
		x.classList.remove("my-selected"+n);
		return get_url_from_selected(x);
	}).join("\n"));
}

function add_new_group_n(new_n){
	selected_css_rules(new_n);
	add_group_btns(get_all_selected, new_n);
	group_indx2is_initialised.push(true);
}

function btn_pressed(e, node){
	e.preventDefault();
	
	const prev_n = parseInt(node.dataset.n);
	
	if (e.which === 3){
		// Right click
		set_node_new_n(node, undefined);
		return;
	}
	
	const new_n = isNaN(prev_n) ? 0 : (prev_n + 1);
	if (!group_indx2is_initialised[new_n])
		add_new_group_n(new_n);
	set_node_new_n(node, new_n);
}

function add_btn_to_node(x, fn){
	const btn = document.createElement("button");
	btn.addEventListener("mouseup", fn);
	btn.innerText = "+";
	btn.classList.add("my-selection-btn");
	x.appendChild(btn);
	x.classList.add("already-added-btn");
	if (x.href !== undefined){
		x.dataset.original_href = x.href;
		x.setAttribute("href", "#preventDefault");
	}
}

function get_all_threads(){
	return Array.from(document.querySelectorAll(thread_css_selector));
}

function add_btns(fn){
	get_all_threads().filter(x => !x.classList.contains("already-added-btn")).map(x => add_btn_to_node(x, fn));
}

add_css(`
.hidden {
	display:none!important;
}
${thread_css_selector} {
	-webkit-transform: translateZ(0);
}
${thread_css_selector} button {
	position:absolute;
	right:0;
	top:0;
	z-index:999;
}
.my-geturls-btn-container {
	position:fixed;
	left:0;
	z-index:9999999;
}
${extra_css}
`);
add_new_group_n(0);
add_btns(e => btn_pressed(e, btn_pressed__getnode(e.target)));

if (add_addbtns_btn){
	place_btn(create_btn("Add Buttons", add_btns));
}

if (hide_thread){
	get_all_threads().map(x => {
		try {
			if (hide_thread(get_url_from_selected(x)))
				x.style.display = "none";
		} catch(e){
			// Most likely one image_wrapper failed to load
			return;
		}
	});
}


}
