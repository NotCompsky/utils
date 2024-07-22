var male_names = ["Greg","Danny","David","Mike","William","Eric","Tim","Steven","Stephen","Derik","Lennie","Shane","Darryl","Robert","Jason","Tom","Logan","Pete","Adam","Chris","Lee","Neil","Scott","Charles","Christian","Brandon","Jerry","Porter","Matt","Julio","Walter","Ron","Peter","Randy","Mark","Justin","Alan","Michael","Timothy","Ryan","Bernard","Jake","Josh","Yosef","Harry","Benjamin","Joseph","Shaun","Wilson","Thomas","Owen","Devin","Joe","James","Joey","Vlad","Marcos","Spencer","Brad","Lloyd","George","Jimmy","Jose","Paul","Samuel","Edward","Matthew","Frank","Rob","Carl","Phil","John","Kevin","Keith","Dave"].map(x => x+" ");
function iterate_over_user(){
	const ls = [];
	for (let node of document.getElementsByTagName("svg")){
		const lss = node.getElementsByTagName("image");
		if (lss.length !== 0){
			ls.push([node,lss[0],node.parentNode.parentNode.parentNode.parentNode.parentNode.parentNode]);
		}
	}
	if (ls.length === 0){
		for (let node of document.getElementsByTagName("img")){
			ls.push([null,null,node.parentNode.parentNode.parentNode.parentNode.parentNode.parentNode.parentNode.parentNode]);
		}
	}
	return ls;
}
// get surnames:
let surnames = [];
for (let [node,svg_img_node,user_container] of iterate_over_user()){
	const name = user_container.getElementsByTagName("a")[1].innerText;
	const surname = name.replace(/^.* /g, "");
	if (!surnames.includes(surname)){
		if (surname.length > 3){
			surnames.push(surname);
		}
	}
}
console.log(surnames.join("\n"));
// highlight male names:
for (let [node,svg_img_node,user_container] of iterate_over_user()){
	const name = user_container.getElementsByTagName("a")[1].innerText;
	let is_male_name = false;
	for (let fname of male_names){
		if (name.startsWith(fname)){
			is_male_name = true;
			break;
		}
	}
	if (is_male_name)
		user_container.classList.add("is_male_name");
	else
		console.log(name);
}
// convert <svg>...<image> into <img>:
for (let [node,svg_img_node,user_container] of iterate_over_user()){
	if (node !== null){
		const img_href = svg_img_node.getAttribute("xlink:href");
		const new_img_node = document.createElement("img");
		new_img_node.src = img_href;
		new_img_node.setAttribute("loading","lazy");
		node.parentNode.appendChild(new_img_node);
		node.remove();
	}
}


// de-convert links
for (let node of document.getElementsByTagName("a")){
	if (node.href.startsWith("http://localhost:8080/cached/http://localhost:8080/groups/483802269549040/user/")){
		node.href = node.href.replace("http://localhost:8080/cached/http://localhost:8080/","https://www.facebook.com/");
	}
}



var req_aa = "aa";
function get_next_aa(){
	let a = req_aa;
	if (a[1] == "z"){
		a[0] = String.fromCharCode(a.charCodeAt(0) + 1);
		a[1] = "0";
	} else (a[1] == "8"){
		a[0] = String.fromCharCode(a.charCodeAt(0) + 1);
		a[1] = "a";
	}
	req_aa = a;
}
function get_next_page(aa, cursor){
	const json_stringified = `av=61562956983263&__aaid=0&__user=61562956983263&__a=1&__req=${req_aa}&__hs=19925.HYP%3Acomet_pkg.2.1..2.1&dpr=1&__ccg=EXCELLENT&__rev=1015040993&__s=8cuuvx%3Abk4j3n%3A25m73c&__hsi=7394217785599622757&__dyn=7AzHK4HwkEng5K8G6EjBAg5S3G2O5U4e2C17xt3odE98K360CEboG0x8bo6u3y4o2Gwfi0LVEtwMw5Qx278c87m221Fwgo9oO0-E4a3a4oaEnxO0Bo7O2l2Utwqo31wiE567Udo5qfK0zEkxe2GewGwkUe9obrwh8lwUwgojUlDw-wSU8o4Wm7-2K0-poarCwLyES0Io88cA0z8c84q58jyUaUcojxK2B08-269wkopg6C13whEeE4WVufxa3mUqwjVqwLwHw&__csr=grgRkIII4lbNAakJisQKGdeSyFZ8yq9EuhiN4JZEgxABEldni4FnsBATEDsQQJ6nlVqRiDrWnjy4iR8Z9WQBgN6WGFkUZuZpmcvJ2QbVbgKFaAQgx99pQVqgycgKm5paXzbBiiyKqiLFa8Dyolm4damaobohixaFaDBxybyVZ2ESh6xy5XykZ2odaCx-uF9oWfzbyFWKWzo88Je1AwEwGyF8py8uwCxe3a1Rw8Ki9wmovxO2TwgE3gxC1ayoG1Pwq8a82bwnosDwbW0jO08IG0Q80iSQ01b5w047iDway0sB6y8KOwrpU17U0xi0drw0x1who720No6q04VS037u01aag0qtg28w2fo1m86y321rK02fO&__comet_req=15&fb_dtsg=NAcNajngCwNSnuqB3oUzNvtW-fuwkAve97dqjhjbz0AB6pWkDXaHnQA%3A36%3A1721599746&jazoest=25764&lsd=IHLZ0nvWtcURRPX-KZurpP&__spin_r=1015040993&__spin_b=trunk&__spin_t=1721600486&qpl_active_flow_ids=431626709&fb_api_caller_class=RelayModern&fb_api_req_friendly_name=GroupsCometMembersPageNewMembersSectionRefetchQuery&variables=%7B%22count%22%3A10%2C%22cursor%22%3A%22${cursor}%22%2C%22groupID%22%3A%22483802269549040%22%2C%22recruitingGroupFilterNonCompliant%22%3Afalse%2C%22scale%22%3A1%2C%22id%22%3A%22483802269549040%22%7D&server_timestamps=true&doc_id=8843517519008531`;
	fetch("https://www.facebook.com/api/graphql/", {mode:"no-cors", method:"POST", body:json_stringified, redirect:"follow"}).then(r => {
		get_next_aa();
		if (r.ok){
			r.json().then(d => {
				const new_members = d["data"]["node"]["new_members"];
				for (let new_member of new_members["edges"]){
					const text = new_member["join_status_text"]["text"];
					const node = JSON.stringify(new_member["node"]);
				}
				const has_next_page = new_members["page_info"]["has_next_page"];
				const end_cursor = new_members["page_info"]["end_cursor"];
				if (has_next_page){
					get_next_page(end_cursor);
				}
			});
		}
	});
}

// facebook.response.json

