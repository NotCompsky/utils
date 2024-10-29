#!/usr/bin/env python3

import os
import re
import pymysql
import json


cursor = None
listfilepath:str = "/tmp/getbookranges.tmp"
pagen_regexp:str = "^([0-9]+)[.](jpe?g|png)$"


def dblqt(s:str):
	return "'" + s.replace('\\','\\\\').replace("'","''") + "'"


def cursor_get_11(cmd:str):
	if cursor is None:
		return None
	cursor.execute(cmd)
	try:
		return cursor.fetchall()[0][0]
	except IndexError:
		return None

def cursor_get(cmd:str):
	if cursor is None:
		return None
	print(cmd)
	cursor.execute(cmd)
	try:
		return cursor.fetchall()
	except IndexError:
		return None


def get_page_ns(dir_path2id:dict, page_ns:list, dirid:int, dirpath:str, is_root:bool):
	n_files_in_tagem:int = 0
	n_used_in_rpill:int = 0
	fnames:str = ""
	pdf_exists:bool = False
	#print(dirpath)
	for entry in os.listdir(dirpath):
		fullpath:str = f"{dirpath}/{entry}"
		m = re.search(pagen_regexp, entry)
		if m is not None:
			page_ns.append(int(m.group(1)))
			fnames += f",{dblqt(entry)}"
		elif os.path.isdir(fullpath):
			dirid2:int = None
			if dirid is not None:
				dirid2 = dir_path2id.get(fullpath+"/") or cursor_get_11(f"SELECT id FROM dir WHERE parent={dirid} AND name={dblqt(entry+'/')} LIMIT 1")
			ls = get_page_ns(dir_path2id, page_ns, dirid2, fullpath, False)
			n_files_in_tagem += ls[1]
			n_used_in_rpill  += ls[2]
		elif entry.endswith(".pdf"):
			pdf_exists = True
		elif entry.endswith(".epub"):
			pdf_exists = True
		elif entry == "_filename_to_boxes.txt":
			with open(fullpath) as f:
				lines = [re.search("^([0-9]+)[.][a-z]+/",x) for x in f.read().split("\n")]
				n_tagged_files:int = len(set([x.group(1) for x in lines if x is not None]))
				n_used_in_rpill += n_tagged_files
				print("n_tagged_files",n_tagged_files,fullpath)
	if (dirid is not None) and (len(fnames) != 0):
		# n_files_in_tagem += cursor_get_11(f"SELECT COUNT(*) FROM file WHERE dir={dirid} AND name IN ({fnames[1:]})")
		'''rows = cursor_get(f"""
			SELECT (f2t.tag IN (115920,235848,236159,235638,236187,236186)) AS ddd, COUNT(DISTINCT f.id)
			FROM file f
			LEFT JOIN file2tag f2t ON f2t.file=f.id AND f2t.tag IN (115920,235848,236159,235638,236187,236186)
			WHERE f.dir={dirid}
			  AND f.name IN ({fnames[1:]})
			GROUP BY ddd
		""")
		for is_used_in_rpill, count in rows:
			n_files_in_tagem += count
			if is_used_in_rpill:
				n_used_in_rpill += count'''
		n_files_in_tagem += cursor_get_11(f"""
			SELECT COUNT(*)
			FROM file f
			WHERE f.dir={dirid}
			  AND f.name IN ({fnames[1:]})
		""")
		n_used_in_rpill += cursor_get_11(f"""
			SELECT COUNT(DISTINCT f.id)
			FROM file f
			JOIN file2tag f2t ON f2t.file=f.id AND f2t.tag IN (115920,235848,236159,235638,236187,236186)
			WHERE f.dir={dirid}
			  AND f.name IN ({fnames[1:]})
		""")
		dir_path2id[dirpath] = dirid
	return (pdf_exists, n_files_in_tagem, n_used_in_rpill)


def get_book_names(rootdir:str, book_names:list, prefix:str):
	get_used_in_rpill:bool = False
	for entry in os.listdir(rootdir):
		if entry == "used_in_rpill":
			get_used_in_rpill = True
			continue
		fullpath:str = f"{rootdir}/{entry}"
		if os.path.isdir(fullpath):
			for entry2 in os.listdir(fullpath):
				sub_fullpath:str = f"{rootdir}/{entry}/{entry2}"
				if os.path.isfile(sub_fullpath) or entry2.startswith("Chapter "):
					book_names.append(f"{prefix}{entry}")
					break
				book_names.append(f"{prefix}{entry}/{entry2}")
	if get_used_in_rpill:
		fff:str = f"{rootdir}/used_in_rpill"
		for dirname in os.listdir(fff):
			get_book_names(f"{fff}/{dirname}", book_names, f"used_in_rpill/{dirname}/")


RED:str   = '\033[0;31m'
WHITE:str = '\033[1;37m'
BLUE:str  = '\033[0;34m'
GREEN:str = '\033[0;32m'
BLANK:str = '\033[0m'


if __name__ == "__main__":
	import argparse
	import json
	
	parser = argparse.ArgumentParser()
	parser.add_argument("rootdir")
	parser.add_argument("dir_path2id_fp", help="/path/to/book__dir_path2id.json (to read from or write to)")
	parser.add_argument("--max-w", type=int, default=os.get_terminal_size().columns-7)
	parser.add_argument("--book", default=None, action="append")
	parser.add_argument("--cached", default=False, action="store_true")
	parser.add_argument("--tagem-user")
	parser.add_argument("--tagem-pwrd")
	args = parser.parse_args()
	
	rootdir:str = args.rootdir
	BOOK_METAS_FP:str = f"{rootdir}/_book_metas.json"
	
	dir_path2id:dict = {}
	try:
		dir_path2id = json.load(open(args.dir_path2id_fp))
	except FileNotFoundError:
		pass
	
	if not args.cached:
		if input("Used cached results? [y = yes]") == "y":
			args.cached = True
	
	if not args.cached:
		if args.tagem_user is not None:
			db = pymysql.connect(db="tagem", user=args.tagem_user, host="localhost", password=args.tagem_pwrd, unix_socket="/run/mysqld/mysqld.sock")
			cursor = db.cursor()
	
	book_metas:list = []
	if args.cached:
		with open(BOOK_METAS_FP,"r") as f:
			book_metas = json.load(f)
	
	book_names:list = args.book
	if args.book is None:
		if args.cached:
			book_names = [x[4] for x in book_metas]
		else:
			book_names = []
			get_book_names(rootdir, book_names, "")
	
	if args.cached:
		book_metas = [x for x in book_metas if x[4] in book_names]
	else:
		for book_name in book_names:
			page_ns:list = []
			bookdir:str = f"{rootdir}/{book_name}"
			dir_id:int = cursor_get_11(f"SELECT id FROM dir WHERE full_path={dblqt(bookdir+'/')} LIMIT 1")
			pdf_exists, n_files_in_tagem, n_used_in_rpill = get_page_ns(dir_path2id, page_ns, dir_id, bookdir, True)
			page_ns = list(set(page_ns))
			page_ns.sort()
			ranges:str = ""
			i:int = 0
			N:int = len(page_ns)
			while i < N:
				ranges += str(page_ns[i])
				while (i+1 < N) and (page_ns[i+1] == page_ns[i]+1):
					i += 1
				ranges += "-" + str(page_ns[i]) + "  "
				i += 1
			book_metas.append((pdf_exists,n_used_in_rpill,n_files_in_tagem,N,book_name,ranges))
		with open(BOOK_METAS_FP,"w") as f:
			json.dump(book_metas, f)
	
	for pdf_exists, n_used_in_rpill, n_files_in_tagem, N, book_name,ranges in sorted(book_metas):
		ranges_ls:list = ranges.split(" ")
		ranges_str:str = ""
		line_len:int = 0
		for rrange in ranges_ls:
			if line_len+len(rrange) > args.max_w:
				ranges_str = ranges_str[:-1]
				ranges_str += '\n\t'
				line_len = 0
			ranges_str += rrange + ' '
			line_len += len(rrange) + 1
		if len(ranges_str) != 0:
			ranges_str = ranges_str[:-1]
		
		meta_str:str = "" if (N==0) else f"\n{GREEN}{n_used_in_rpill}{WHITE}{n_files_in_tagem}{BLUE}{N}{BLANK}\t{ranges_str}"
		if pdf_exists:
			meta_str += f"\t{GREEN}PDF{BLANK}"
		print(f"{WHITE}{book_name}{BLANK}{meta_str}")
	
	json.dump(dir_path2id, open(args.dir_path2id_fp,"w"))
