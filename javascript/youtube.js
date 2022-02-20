// ==UserScript==
// @name	highlight seen videos
// @namespace	https://goooooooooooooooooooooooooooooooooooooooooooooooooooooooogle.com
// @description	Highlight vids
// @include	https://www.youtube.com/*
// @version 0.0.1
// ==/UserScript==


const ph_ids = [
	// MrBallen one-off videos:
	'_aZBRT4PeHQ', // former Pentagon official was beaten to death and left in a landfill
	'WnPVedz7NQ4',
	'9bvMIHUVkVE',
	'ErNOKNxoHQ8',
	'ny_s07D-LT8',
	'XrFFRrsWBNc',
	'F9PCCZwKHE4',
	'5EobUTGG1ng',
	'IB9-eS44A_8',
	'DsIx9xZ88PQ',
	'k7lytyRgBwE',
	'_SeAJTpOCbo',
	'4qMcCXOnEYY',
	'ScqRv9CpJWs',
	'b4iNqoLQ-1E',
	'YMXVG2Y7vaw',
	'Pyj8FKZaW0A',
	'5vGtcfJOPOY',
	'endyy_4jqnA',
	'eWFUF3VIm-4',
	'xSJoeTOtOMA',
	'fLs9za4X08E',
	'kkFuZ-_sPP8',
	'26Ln8V6og-c',
	'CTwhTjFgSKk',
	'jQY5kZKKmbg',
	'dxq2m8bE690',
	'BBOakDDZXaM',
	'tjEPYA6m3dM',
	'HiYLxGLdKMk',
	'lw-sSg50kp0',
	'enHQJK1tPkc',
	'sx14YtiLFIM',
	'pfnbgIkT_jk',
	'MC8ZfyNBPu0',
	'DxjVH0HC1Ws',
	'sFV6R9GgPy4',
	'A6IgVVi8TtI',
	'Tul0S0xFzh8',
	'Uk8NEkZoJWc',
	'YJm1XUoDWcE',
	'_PXwLa6mTEA',
	'7G8f-AMx9-w',
	'u-upHQYZbTs',
	'PWlUUs4bKQQ',
	'kNkA9BRRwpY',
	'KdQujjxb258',
	'Nz5Gw2vBtgs',
	'PiUo8biy0Ok',
	'q86aQiINdyo',
	'f51b7l9qV3k',
	'OPfcCindxhg',
	'Yh3nCT4RGsY',
	'9zu2ctUEJdE',
	'QRzgmZiZn6U',
	'wekABO_hraI',
	'Y3quprQlOXI',
	'p3e2NQdki-8',
	'jQ3ZBEBusCg',
	'8MpR4k3-edc',
	'xYUKVwP-AVQ',
	'pECd0uXB0RI',
	'4QiSJMGDF1A',
	'l65EL29NTjM',
	'WO28hxU6qjs',
	'yQDnwbav-cE',
	'joXznzIrB9c',
	'5jDbJbCuKl4',
	'A3yaednyAjI',
	'vjpc82b2CVQ',
	'FRnPwaR_CW8',
	'iuoVrgJhc1E',
	'zrVObXVuwJw',
	'RseUuG5Kl8Q',
	'S9qKjIUsCWY',
	'OgL41aUL7tY',
	'UR2lSI12DA8',
	'FjxBndqU0kM',
	'-8gIahvAcas',
	'oehVw_YAkJI',
	'uIvIBKiQhVM',
	'xtEfMtdnjo0',
	'-yNVelnkfZo',
	'YA1YStQa2Vg',
	'NYbUTIwWdNI',
	'QTn8c7dXIro',
	'yL35e5sr0Ac',
	'PxM3Y201p2s',
	'A1WDu6_0bi0',
	'Er9G5yRNYj4',
	'HQdX4-EQoTE',
	'fB3KCp33ELk',
	'kqxtK_mrCdo',
	'-1ZDeG7clm8',
	'na4Rlu_LBbY',
	
	// Lost Episode N
	'yjJqvSLtTLE', // 5
	'7mu331vmZqQ', // 6
	'ukT0V1q1JOI', // 7
	'gNzuVpVFY1Q', // 8
	'FYAm5_85Ls0', // 9
	
	// ___ | Strange but true (Pt. N)
	'aJJmwCKgHAQ', // 15
	
	// 411 stuff
	'ogjgVGtkzJc',
	'nPvAKS7b1d4',
	'_cSpBqXj_X8',
	
	// SCARIEST places people got STUCK | Halloween Scare-A-Thon (part N)
	'M6Gh1QNVDU8', // 3
	'kODsF75lAJY', // 6
	'6bX6as944uo', // 11
	
	// Historical Legends Part N
	'pCvx4jikmsI', // 5
	
	// INSANE survival stories | Part N
	'qO6EAw8kVpA', // 1
	
	// ___ | Missing 411 (Part N)
	'_cSpBqXj_X8', // First one?? No part number
	'nPvAKS7b1d4', // 2
	'tbcxPOgriJU', // 3
	'tKqLEc6UsRE', // 4
	'FE_C035m-Z8', // 6
	'6uB15F9zZJY', // 7
	'Ytr3dbjrx4Q', // 8
	'idNX6Dm6li8', // 9
	'6ZH3y8y82dY', // 10
	'yklVdWnY4uE', // 11
	'ODIGopvDSXA', // 12
	'gdONLZ4EuO8', // 13
	'XZCCCxnSMxs', // 14
	'd7iRyj6pvJE', // 15
	'YNpctf6WJx8', // 16
	'Cf9ahZB9yfY', // 17
	'kExkpDOGf0s', // 18
	'F6kNbhD62-0', // 19
	'm-Kvc3wbSfw', // 20
	'wSiHjKIckNM', // 21
	'UR5ZTc9ZwxU', // 22
	'bhOpd5lJU4Y', // 23
	
	// Top 3 SCARIEST true stories | Spooky Stories Part N
	'7gFlFR1NBFE', // 1
	
	// ___ | Unsolvable Mysteries Part N
	'phJD8mb0Tfs', // 1
	'aD_Ot9aXy7s', // 2
	
	// ___ -- SOLVED Mysteries, Episode N
	'iI5chSkHjwg', // 1
	'vihYdB3SkuE', // 2
	'GCIZ1mnWAHw', // 3, The WEIRDEST murder motive EVER
	'b4iNqoLQ-1E', // 4
	
	// Top 3 CRAZY ways people escaped DEATH | Part N
	'U36jtuJVBjQ', // 2
	'yPE84ZJBJWk', // 3
	
	// Top 3 stories that sound fake but are 100% real | Part N
	'ZUOehfFdj1Y', // 1
	'4qYqpyVZOH4', // 2
	'l74_VQPdxjU', // 4
	'Zaa53S-8lDM', // 5
	'lG6ghojk3oQ', // 6
	'WWWWMdm9ZY4', // 7
	'xUxAsyLN4yI', // 8
	'vjpc82b2CVQ', // 9
	'VzMwI_KMjr8', // 10
	'XUXHUjsb6O4', // 11
	'RK6MIl28b-8', // 12
	'C2FVjH4WWlU', // 14
	'Pzm-T_btfa0', // 16
	
	// Top M videos with DISTURBING backstories | Part N
	'eKLsETk-o4g', // 1
	'y156BNu3S5Y', // 2
	'QoZKJiar_Uc', // 4
	'5-DiRM4CFPI', // 5
	'qw9UOmib5hE', // 6
	'vARuVoqbQL0', // 7
	'MUyBjJciy80', // 9
	
	// Top 3 photos with DISTURBING backstories | Part N
	'7tgiVW9mSL8', // 1
	'DvLyk4e0SUk', // 2
	'Fh8yD0fyHtA', // 3
	'l4MGIA0A5VM', // 4
	'pHkpDWBmRO8', // 5
	'wz6F2rDngvA', // 6
	'lMOYAtqb_Kc', // 7
	'VBjbkNZcoHo', // 9
	'NhxvpeAzSsU', // 10
	'6rpQ3VP9-Mc', // 11
	'AGjdQlwzbkk', // 12
	'Mpv4i32EgeA', // 13
	'tnZT8LFRpYY', // 14
	'L2u2_q4tFp8', // 17
	'BqhmFvoSc34', // 18
	'zhgBeoviM8Y', // 19
	'maskb9lXkuU', // 20
	
	// Top 3 last minute decisions that changed history | Part N
	'Ua0HMlqo-hI', // 1
	
	// Top 3 places you CAN'T GO & people who went anyways... | Part N
	'ZiOyKVcDODo', // 1
	'-lOsTCBg4fQ', // 2
	'4rWMEiC43pM', // 3
	'zgBXb3W71DY', // 4
	'IDJ8_VFtexw', // 5
	'8swB7qMU7gk', // 6
	'ZUkAkMJ9c18', // 8
	'NtDAo7jS8Z4', // 9
	'FTrEzqm6dCE', // 10
	'K7cwHK5gVR8', // 11
	'U9en1PDgpAI', // 12
	'RWXDmtd2Zq0', // 13 (reupload)
	'o-ywxRasv5s', // 14
	'PdDgIl1vNj4', // 15
	'TZ8EGOEc7uw', // 16
	'32Nwh9sq7DI', // 17
	'dS302hMEzxQ', // 18
	'YVMTWygVl5c', // 19
	'toswhS4ehRw', // 20 (and survived)
	'fMaWaiWRs_4', // 21
	'242QjjilqIE', // 22
	'XUFf4QGj0kY', // 23
	'6apkM24DhGI', // 24
	'EQFYw9sRGzc', // 25
	'K97vxBCpwx8', // 26
];


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
    return btn;
};

add_css(`#custom-btn-container {
    position:fixed;
    top:1em;
    left:1em;
    z-index:9999;
}`);

const show_infos = () => {
	for (let node of document.getElementsByClassName("yt-simple-endpoint")){
		const title_node = node; //.getElementsByTagName("a")[0];
		const match = title_node.href.match("https://www.youtube.com/watch[?]v=([A-Za-z0-9_-]+)(&t=[0-9]+s)?$"); // Must match the end too because comments have permalinks that are the video URL plus a hash string
		if (match === null)
			continue;
		if (ph_ids.includes(match[1])){
			node.style.background = "purple";
			continue;
		}
		const title_text = title_node.innerText.replace(/^\n +/,"").replace(/ +$/,"").toLowerCase();
		if (title_text.match(/poop/))
			title_node.style.color="blue";
	}
	const is_prev_seen = (ph_ids.includes(document.location.href.match("https://www.youtube.com/watch[?]v=([A-Za-z0-9_-]+)")[1]));
	for (let node of document.getElementsByTagName("h1")){
		node.style.color = ["","purple"][is_prev_seen];
	}
}

add_btn("Show Infos", show_infos);
show_infos();
