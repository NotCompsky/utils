#include <dirent.h>
#include <unistd.h>
#include <compsky/asciify/asciify.hpp>
#include <compsky/utils/ptrdiff.hpp>


extern "C"
bool del_direct_child_files_in_dir(const char* const dirpath){
	struct dirent* ent;
	DIR* const dir = opendir(dirpath);
	if (dir == nullptr){
		return true;
	}
	const int dirfd_value = dirfd(dir);
	bool rc = false;
	while ((ent = readdir(dir)) != nullptr){
		if (ent->d_type == DT_REG){
			// Delete file:
			if (unlinkat(dirfd_value, ent->d_name, 0) != 0){
				rc = true;
				break;
			}
		}
	}
	closedir(dir);
	return rc;
}


char* cprint_buf;
char* cprint_itr;

extern "C"
void cprint_init(const std::size_t sz){
	cprint_buf = reinterpret_cast<char*>(malloc(sz));
	cprint_itr = cprint_buf;
}

extern "C"
void cprint_out(const int fd){
	write(fd, cprint_buf, compsky::utils::ptrdiff(cprint_itr,cprint_buf));
}

extern "C"
void cprint1b(const char* const str1){
	compsky::asciify::asciify(cprint_itr, str1, '\n');
}

extern "C"
void cprint2b(const char* const str1,  const char* const str2){
	compsky::asciify::asciify(cprint_itr, str1, str2, '\n');
}

extern "C"
void cprint3b(const char* const str1,  const char* const str2,  const char* const str3){
	compsky::asciify::asciify(cprint_itr, str1, str2, str3, '\n');
}

extern "C"
void cprint4b(const char* const str1,  const char* const str2,  const char* const str3,  const char* const str4){
	compsky::asciify::asciify(cprint_itr, str1, str2, str3, str4, '\n');
}

extern "C"
void cprint5b(const char* const str1,  const char* const str2,  const char* const str3,  const char* const str4,  const char* const str5){
	compsky::asciify::asciify(cprint_itr, str1, str2, str3, str4, str5, '\n');
}
