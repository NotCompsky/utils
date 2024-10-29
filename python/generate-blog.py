#!/usr/bin/env python3

import subprocess
import re

def getslub(basename:str):
	return re.sub("[^a-z0-9]","-",basename.lower())

def md2html(path_to_md2html:str, fp:str, mainlinks_html:str):
	out_fp:str = "/tmp/agray.org.blog.html.tmp"
	subprocess.run([path_to_md2html,fp,out_fp])
	contents:str = None
	with open(out_fp,"r") as f:
		contents = f.read()
	os.remove(out_fp)
	current_id_str_indx:int = 0
	toc_htmls:list = ["","",""]
	def headers_to_toc_re_fn(m):
		nonlocal current_id_str_indx
		idstr:str = "h"+str(current_id_str_indx)
		current_id_str_indx += 1
		header_level, title = m.groups()
		s:str = '<a class="sidebar_link" href="#'+idstr+'">'+("*"*(int(header_level)-1))+title+'</a>'
		toc_htmls[0] += s
		if int(header_level) <= 2:
			if int(header_level) == 1:
				s = '</div><div class="header_block"><div class="sidebar_link" href="#'+idstr+'">'+title+'</div>'
			else:
				s = '<a class="sidebar_link" href="#'+idstr+'">'+("*"*(int(header_level)-2))+title+'</a>'
			toc_htmls[2] += s
			if int(header_level) <= 1:
				toc_htmls[1] += s
		return '<h'+header_level+' id="'+idstr+'">'+title+'</h'+header_level+'>'
	if len(toc_htmls[1]) != 0:
		toc_htmls[1] = toc_htmls[1][6:] + "</div>"
	if len(toc_htmls[2]) != 0:
		toc_htmls[2] = toc_htmls[2][6:] + "</div>"
	contents = re.sub("<h([0-9]+)>((?:[^<]|<[^/]|</[^h]|</h[^0-9])+)</h[0-9]+>", headers_to_toc_re_fn, contents)
	contents = contents.replace("TABLE_OF_CONTENTS_PLACEHOLDER",toc_htmls[0]).replace("TABLE_OF_CONTENTS_UP_TO_H1_PLACEHOLDER",toc_htmls[1]).replace("TABLE_OF_CONTENTS_UP_TO_H2_PLACEHOLDER",toc_htmls[2]).replace("LIST_OF_HTML_FILES",mainlinks_html).replace("<title></title>",'<title></title><link rel="stylesheet" href="styl.css"></link>')
	contents = re.sub("<p>[ \t]*</p>", "", contents).replace("\t","")
	return contents

if __name__ == "__main__":
	import argparse
	import os
	
	parser = argparse.ArgumentParser()
	parser.add_argument("--mainlinks-html-file")
	parser.add_argument("--overwrite", default=False, action="store_true")
	parser.add_argument("--md_to_html", default="md_to_html", help="/path/to/md_to_html binary from https://agray.org/repos/md-to-html")
	parser.add_argument("srcdir")
	parser.add_argument("outdir")
	args = parser.parse_args()
	
	mainlinks_html:str = '<a class="sidebar_link" href=".">Blog Home</a>'
	if (args.mainlinks_html_file is not None) and os.path.exists(args.mainlinks_html_file):
		with open(args.mainlinks_html_file,"r") as f:
			mainlinks_html = f.read()
	else:
		for fname in os.listdir(args.srcdir):
			if fname.endswith(".md"):
				basename:str = fname[:-3]
				mainlinks_html += '<a class="sidebar_link" href="'+getslub(basename)+'.html">'+basename+'</a>'
	
	previews_of_all_content_files:str = ""
	for fname in os.listdir(args.srcdir):
		if fname.endswith(".md"):
			basename:str = fname[:-3]
			slug:str = getslub(basename)
			dest_fp:str = args.outdir+'/'+slug+'.html'
			if (not args.overwrite) and os.path.isfile(dest_fp):
				print("Skipping file that already exists: " + dest_fp)
			else:
				contents:str = md2html(args.md_to_html, args.srcdir+'/'+fname, mainlinks_html)
				with open(dest_fp, "w") as f:
					f.write(contents)
			previews_of_all_content_files += '<div class="content_entry"><h2>'+basename+'</h2><a href="'+slug+'.html">Click here to continue reading</a></div>'
	with open(args.outdir+"/index.html","w") as f:
		f.write("""<!DOCTYPE html>
<html>
<head>
	<link rel="stylesheet" href="styl.css"></link>
	<title>Blog</title>
</head>
<body>
	<div id="sidebar">
		<img id="profileimg" src="img/me1.jpg">
		<div class="header_block">
			<h2>A Gray</h2>
			<a class="sidebar_link" href="..">Home</a>
		</div>
		<div class="header_block">LIST_OF_HTML_FILES</div>
	</div>
	<div id="content_container">PREVIEWS_OF_ALL_CONTENT_FILES</div>
</body>
</html>""".replace("LIST_OF_HTML_FILES",mainlinks_html).replace("PREVIEWS_OF_ALL_CONTENT_FILES",previews_of_all_content_files))