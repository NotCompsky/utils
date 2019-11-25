#include "countdir.h"
#include <stdio.h>


size_t count_dir(const char* const path){
	size_t n = 0;
	
	DIR* dir;
	struct dirent* ent;
	
	dir = opendir(path);
	while ((ent = readdir(dir))){
		++n;
#ifdef RECURSIVE
		const auto _type = ent->d_type;
		const char* const _path = ent->d_name;
		
		if (
			_type == DT_DIR  &&  (
				_path[0] != '.' ||  (
					_path[1] != 0  &&  (
						_path[1] != '.'  ||  _path[2] != 0
					)
				)
			)
		){
			// WARNING: Not supported by all filesystems
			n += count_dir(_path);
		}
#endif
	}
	closedir(dir);
	
	return n - 2; // Ignore . and ..
}

int main(const int argc,  const char** const argv){
	size_t n = 0;
	for (auto i = 1;  i < argc;  ++i)
		n += count_dir(argv[i]);
	printf("%lu\n", n);
	return 0;
}
