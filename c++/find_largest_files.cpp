#include <dirent.h> // for opendir, readdir
#include <algorithm> // for std::sort
#include <cstring> // for memcpy
#include <vector> // for std::vector
#include <cstdio> // for printf
#include <sys/stat.h>


struct FileSize {
	size_t size;
	char path[1010];
	FileSize(const char* const _path,  const size_t _size)
	: size(_size)
	{
		memcpy(this->path,  _path,  strlen(_path) + 1);
	}
};
bool operator<(FileSize& a,  FileSize& b){
	return (a.size > b.size);
}


std::vector<FileSize> file_size_pairs;


void traverse(const char* path, const int path_len, const int depth){
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
			traverse(epath, epath_len, depth + 1);
			continue;
		}
		
		static struct stat st;
		stat(epath, &st);
		file_size_pairs.emplace_back(reinterpret_cast<const char*>(epath), st.st_size);
		}
	}
	closedir(dir);
}


int main(const int argc,  const char** argv){
	file_size_pairs.reserve(4096 * 4);
	for (int i=1; i<argc; ++i)
		traverse(argv[i], strlen(argv[i]), 0);
	std::sort(file_size_pairs.begin(), file_size_pairs.end());
	for (const FileSize& fs : file_size_pairs){
		printf("%s\n", fs.path);
	}
	return 0;
}
