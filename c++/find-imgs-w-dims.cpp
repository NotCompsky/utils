#include <cstdio>
#include <unistd.h>
#include <cstring>
#include <compsky/os/metadata.hpp>
#include <compsky/deasciify/a2n.hpp>
#include <compsky/mimetype/metadata.hpp>
#include <compsky/os/read.hpp>


unsigned MIN_W;
unsigned MIN_H;


bool str_same_except_left_ends_with_slash(const char* a,  const char* b){
	// printf("%s ==?== %s\n", a, b);
	while((*a == *b) and (*b != 0)){
		++a;
		++b;
	}
	return ((*b == 0) and (*a == '/'));
}


void process_file(char full_path_buf[4096],  unsigned full_path_offset,  const char* const filename){
	const unsigned dirname_strlen = strlen(filename);
	memcpy(full_path_buf+full_path_offset, filename, dirname_strlen);
	full_path_buf[full_path_offset+dirname_strlen  ] = '\0';
	
	char buf[29];
	
	compsky::os::ReadOnlyFile file(full_path_buf);
	if (unlikely(file.is_null() or (file.size<0>() < sizeof(buf)))){
		return;
	}
	
	unsigned w = 0;
	unsigned h = 0;
	file.read_into_buf(buf, sizeof(buf));
	compsky::mimetyp::get_media_dimensions_from_buf(buf, w, h);
	
	if ((w >= MIN_W) and (h >= MIN_H)){
		printf("%s\n", full_path_buf); fflush(stdout);
	} else if (w != 0) {
		printf("%u x %u\n", w, h);
	}
}


void process_dir(char full_path_buf[4096],  unsigned full_path_offset,  const char* const dirname){
	DIR* dir;
	struct dirent* ent;
	
	const unsigned dirname_strlen = strlen(dirname)+1;
	memcpy(full_path_buf+full_path_offset, dirname, dirname_strlen-1);
	full_path_buf[full_path_offset+dirname_strlen-1] = '/';
	full_path_buf[full_path_offset+dirname_strlen  ] = '\0';
	// printf("full_path_buf[%lu]\t%s\n", full_path_buf); fflush(stdout);
	
	dir = opendir(full_path_buf);
	while ((ent = readdir(dir))){
		if (ent->d_type == DT_DIR){
			if ((ent->d_name[0] == '.') and ((ent->d_name[1] == 0) or ((ent->d_name[1] == '.') and (ent->d_name[2] == 0))))
				continue;
			process_dir(full_path_buf, full_path_offset+dirname_strlen, ent->d_name);
		} else if (ent->d_type == DT_REG){
			process_file(full_path_buf, full_path_offset+dirname_strlen, ent->d_name);
		}
	}
	closedir(dir);
}


int main(const int argc,  char* const* const argv){
	if (argc != 4){
		printf("USAGE: %s [DIR_PATH] [MIN_W] [MIN_H]\n", argv[0]);
		return 1;
	}
	
	MIN_W = a2n<unsigned>(argv[2]);
	MIN_H = a2n<unsigned>(argv[3]);
	
	char* const init_dirpath = argv[1];
	char full_path_buf[4096];
	unsigned full_path_offset = 0;
	unsigned firstdirpath_strlen = strlen(init_dirpath);
	if ((firstdirpath_strlen != 1) and (init_dirpath[firstdirpath_strlen-1] == '/')){
		init_dirpath[firstdirpath_strlen-1] = 0;
		--firstdirpath_strlen;
	}
	if (init_dirpath[0] != '/'){
		getcwd(full_path_buf, sizeof(full_path_buf));
		full_path_offset = strlen(full_path_buf);
		full_path_buf[full_path_offset] = 0;
		if (firstdirpath_strlen == 1){
			// path == .
			init_dirpath[0] = 0;
		} else {
			memcpy(init_dirpath, init_dirpath+2, full_path_offset-2); // includes the trailing \0
		}
	}
	
	char buf[36+2*sizeof(full_path_buf)+9];
	const char* const init_dirpath2 = (init_dirpath[0]=='/') ? init_dirpath : full_path_buf;
	
	process_dir(full_path_buf, full_path_offset, init_dirpath);
}
