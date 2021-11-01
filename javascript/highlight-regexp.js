const node_contains_classname = "my-matched-regexp";

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
}
.${node_contains_classname} {
	background:cyan;
	color:red;
}`);

function hlmatches(str_pattern){
	const pattern = new RegExp(`(${str_pattern})`, 'g');
	Array.from(document.getElementsByClassName(node_contains_classname)).map(x => {
		//x.classList.remove(node_contains_classname)
		x.parentNode.innerText = x.parentNode.innerText;
	});
	for (let node of Array.from(document.getElementsByTagName("*"))){
		if (node.children.length !== 0)
			continue;
		if (node.innerText.search(str_pattern) !== -1){
			/*node.classList.add(node_contains_classname);*/
			if (node.classList.contains(node_contains_classname))
				continue;
			node.innerHTML = node.innerHTML.replace(pattern, `<b class="${node_contains_classname}">$1</b>`);
		}
	}
}
function hlnames(){
	hlmatches("[A-Z][a-züş]+ [A-Z][a-züş]+");
}
