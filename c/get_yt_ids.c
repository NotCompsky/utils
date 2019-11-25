#include <dirent.h> // for opendir, readdir
#include <string.h> // for strlen, memcpy
#include <stdlib.h> // for malloc, realloc
#include <unistd.h> // for write

#define STDOUT_FILENO 1
#define STDERR_FILENO 2


const char SEPARATOR = '|';


void find(const char* path, unsigned int depth){
	DIR* dir;
	dir = opendir(path);
	if (dir == 0)
		return;
	struct dirent* e;
	while (e=readdir(dir) != 0){
		const char* ename = e->d_name;
		if (ename == 0)
			continue;
		if ((ename[0]=='.') &&
			(ename[1]==0 || (ename[1]=='.' && ename[2]==0)))
			// Ignore . and ..
			continue;
		if (e->d_type == DT_DIR){
			find(ename, depth+1);
			continue;
		}
	}
	closedir(dir);
}

int media_exts(const char* str, int i){
	switch(str[--i]){
		case '3':
			switch(str[--i]){
				case 'p':
					switch(str[--i]){
						case 'm':
							switch(str[--i]){
								case '.':
									return 4; // 3pm.
								default: return 0;
							}
						default: return 0;
					}
				default: return 0;
			}
		case '4':
			switch(str[--i]){
				case 'p':
					switch(str[--i]){
						case 'm':
							switch(str[--i]){
								case '.':
									return 4; // 4pm.
								default: return 0;
							}
						default: return 0;
					}
				default: return 0;
			}
		case 'b':
			switch(str[--i]){
				case 'o':
					switch(str[--i]){
						case 'v':
							switch(str[--i]){
								case '.':
									return 4; // bov.
								default: return 0;
							}
						default: return 0;
					}
				default: return 0;
			}
		case 'f':
			switch(str[--i]){
				case 'i':
					switch(str[--i]){
						case 'g':
							switch(str[--i]){
								case '.':
									return 4; // fig.
								default: return 0;
							}
						default: return 0;
					}
				default: return 0;
			}
		case 'g':
			switch(str[--i]){
				case 'p':
					switch(str[--i]){
						case 'm':
							switch(str[--i]){
								case '.':
									return 4; // gpm.
								default: return 0;
							}
						default: return 0;
					}
				default: return 0;
			}
		case 'i':
			switch(str[--i]){
				case 'v':
					switch(str[--i]){
						case 'a':
							switch(str[--i]){
								case '.':
									return 4; // iva.
								default: return 0;
							}
						default: return 0;
					}
				default: return 0;
			}
		case 'm':
			switch(str[--i]){
				case 'b':
					switch(str[--i]){
						case 'e':
							switch(str[--i]){
								case 'w':
									switch(str[--i]){
										case '.':
											return 5; // mbew.
										default: return 0;
									}
								default: return 0;
							}
						default: return 0;
					}
				default: return 0;
			}
		case 'v':
			switch(str[--i]){
				case '2':
					switch(str[--i]){
						case 'm':
							switch(str[--i]){
								case '.':
									return 4; // v2m.
								default: return 0;
							}
						default: return 0;
					}
				case '4':
					switch(str[--i]){
						case 'm':
							switch(str[--i]){
								case '.':
									return 4; // v4m.
								default: return 0;
							}
						default: return 0;
					}
				case 'g':
					switch(str[--i]){
						case 'o':
							switch(str[--i]){
								case '.':
									return 4; // vgo.
								default: return 0;
							}
						default: return 0;
					}
				case 'k':
					switch(str[--i]){
						case 'm':
							switch(str[--i]){
								case '.':
									return 4; // vkm.
								default: return 0;
							}
						default: return 0;
					}
				case 'l':
					switch(str[--i]){
						case 'f':
							switch(str[--i]){
								case '.':
									return 4; // vlf.
								default: return 0;
							}
						default: return 0;
					}
				case 'm':
					switch(str[--i]){
						case 'w':
							switch(str[--i]){
								case '.':
									return 4; // vmw.
								default: return 0;
							}
						default: return 0;
					}
				case 'o':
					switch(str[--i]){
						case 'm':
							switch(str[--i]){
								case '.':
									return 4; // vom.
								default: return 0;
							}
						default: return 0;
					}
				default: return 0;
			}
		default: return 0;
	}
}


const int zero = 0;
const char endline = '\n';
int last_depth = 0;


char* BUF;
size_t BUF_SZ = 100*12;
size_t BUF_USED = 0;


void enlarge_buf(){
	BUF_SZ += (11 + 1);
	BUF_SZ *= 2;
	BUF = (char*)realloc(BUF, BUF_SZ);
}


void report_yt_id(const char* fpath,  const char* fname,  const int ename_len,  const int ext_len){
	if (BUF_SZ  ==  BUF_USED)
		enlarge_buf();
	auto i = ename_len - ext_len - 11;
	auto j = 0;
	if (fname[ename_len - ext_len - 12]  !=  '-')
		goto goto__lazyerrthinghere;
	for (;  j < 11;  ++i, ++j){
		if (
			('a' <= fname[i]  &&  fname[i] <= 'z') ||
			('A' <= fname[i]  &&  fname[i] <= 'Z') ||
			('0' <= fname[i]  &&  fname[i] <= '9') ||
			(fname[i] == '-') ||
			(fname[i] == '_')
		)
			BUF[BUF_USED++] = fname[i];
		else {
			goto__lazyerrthinghere:
			BUF_USED -= j;
			write(STDERR_FILENO,  fpath,	strlen(fpath));
			write(STDERR_FILENO,  &endline, 1);
			return;
		}
	}
	BUF[BUF_USED++] = SEPARATOR;
}


void find_medias(const char* path, const int path_len, const int depth){
	DIR* dir;
	dir = opendir(path);
	
	if (dir == 0)
		return;
	struct dirent* e;
	
	while ((e=readdir(dir)) != 0){
		const char* ename = e->d_name;
		
		if (ename == 0)
			continue;
		
		if (ename[0] == '.'){
			if (ename[1] == 0)
				continue;
			if (ename[1] == '.')
				continue;
		}
		
		const int ename_len = strlen(ename);
		const size_t epath_len = path_len + ename_len + 1;
		{
			// Scope to keep non-const 'epath' out of scope of goto jump destination
		char epath[epath_len + 1];
		memcpy(epath,  path,  path_len);
		epath[path_len] = '/';
		memcpy(epath + path_len + 1,  ename,  ename_len);
		epath[path_len + 1 + ename_len] = 0;
		
		if (e->d_type == DT_DIR){
			find_medias(epath, epath_len, depth + 1);
			continue;
		}
		
		auto ext_len = media_exts(ename, ename_len);
		if (ext_len == 0)
			continue;
		
		report_yt_id(epath, ename, ename_len, ext_len);
		}
	}
	goto__end:
	closedir(dir);
}

int main(const int argc,  const char** argv){
	BUF = (char*)malloc(BUF_SZ);
	for (int i=1; i<argc; ++i)
		find_medias(argv[i], strlen(argv[i]), 0);
	write(STDOUT_FILENO,  BUF,  BUF_USED);
	free(BUF);
	return 0;
}
