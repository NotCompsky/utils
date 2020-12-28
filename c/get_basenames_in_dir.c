#include <dirent.h> // for opendir, readdir
#include <string.h> // for strlen, memcpy
#include <stdlib.h> // for malloc, realloc
#include <unistd.h> // for write

#define STDOUT_FILENO 1
#define STDERR_FILENO 2


const char SEPARATOR = '\n';


char* BUF;
size_t BUF_SZ = 4096;
size_t BUF_USED = 0;


void enlarge_buf(){
	BUF_SZ += (11 + 1);
	BUF_SZ *= 2;
	BUF = (char*)realloc(BUF, BUF_SZ);
}


void report_yt_id(const char* fpath,  const char* fname,  const int ename_len){
	if (BUF_SZ  <=  BUF_USED + strlen(fname))
		enlarge_buf();
	while((*fname != '.') && (*fname != 0)){
		BUF[BUF_USED++] = *fname;
		++fname;
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
			char epath[epath_len + 1];
			memcpy(epath,  path,  path_len);
			epath[path_len] = '/';
			memcpy(epath + path_len + 1,  ename,  ename_len);
			epath[path_len + 1 + ename_len] = 0;
			
			if (e->d_type == DT_DIR){
				find_medias(epath, epath_len, depth + 1);
				continue;
			}
			
			report_yt_id(epath, ename, ename_len);
		}
	}
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
