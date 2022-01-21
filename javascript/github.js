const dl_text_as_file = (filename, text) => {
	const a = document.createElement("a");
	a.href = URL.createObjectURL(new Blob([text], {type: "text/plain"}));
	a.download = filename;
	a.hidden = true;
	document.body.appendChild(a);
	a.click();
};
const dl___github_displayedcsv = (which_columns) => {
	let s = "";
	for (let node of document.getElementsByClassName("csv-data")[0].getElementsByTagName("tr")){
		const cols = node.getElementsByTagName("td");
		let i = 0;
		for (i = 0;  i < cols.length;  ++i){
			if (which_columns.includes(i))
				s += cols[i].innerText + "\t";
		}
		s += "\n";
	}
	dl_text_as_file("github.csv", s);
};
dl___github_displayedcsv([5,10,21]);
