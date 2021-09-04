alert(Array.from(document.getElementsByClassName("search-result")).map(x => [
	parseInt(x.getElementsByClassName("search-score")[0].innerText.replace(",","").split(" points")[0]),
	x.getElementsByClassName("search-link")[0].href,
	x.getElementsByClassName("search-title")[0].innerText.match(/[\[ ({)\]]([0-9]+) ?[×xX*] ?([0-9]+)(px)?([\]. )}]|$)/).map(x => parseInt(x)).filter(x => x)
]).filter(x => x[2][0]*x[2][1] >= 1920*1080).sort((a,b) => b[0]-a[0]).map(x => x[1]).join("\n"));
