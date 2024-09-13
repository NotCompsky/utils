var s = "";

for (var x of document.getElementsByTagName('h5')){
	s += '\n' + x.innerHTML;
	var y = x.nextSibling.firstChild;
	s += '\n' + y.dataset.tooltipContent;
	s += '\n' + y.lastChild.firstChild.innerHTML;
};

console.log(s); 
