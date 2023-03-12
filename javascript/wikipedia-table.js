function counttblcolumn(tblnode, colindx){
	let count = 0;
	for (let node of tblnode.getElementsByTagName("tr")){
		count += node.childNodes[colindx].innerText.replaceAll(',','')|0;
	}
	return count;
}
counttblcolumn(temp0, 5);
