#include "countdir.h"
#include <unistd.h> // for write
#include <stdint.h> // for uintptr_t
#include <dirent.h> // for DIR


uint32_t count_dir(const char* const path){
	uint32_t n = 0;
	
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
	uint32_t n = 0;
	for (auto i = 1;  i < argc;  ++i)
		n += count_dir(argv[i]);
	char buf[20];
	uint32_t m = n;
	uint32_t n_digits = 0;
	do {
		m /= 10;
		++n_digits;
	} while (m != 0);
	char* itr = buf+n_digits;
	do {
		*(--itr) = '0'+(n%10);
		n /= 10;
	} while (n != 0);
	write(1, buf, n_digits);
	return 0;
}
