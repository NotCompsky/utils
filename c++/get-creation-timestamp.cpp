#include <sys/stat.h>
#include <compsky/asciify/asciify.hpp>
#include <compsky/utils/ptrdiff.hpp>


int main(const int argc,  const char** argv){
	struct statx stat_buf;
	char buf[1024];
	char* itr = buf;
	
	for (int i = 1;  i < argc;  ++i){
		if (statx(0, argv[i], 0, STATX_BTIME, &stat_buf) == -1){
			return 1;
		}
		
		uint64_t probable_creation_time = stat_buf.stx_btime.tv_sec; // inode creation time
		if (stat_buf.stx_mtime.tv_sec < probable_creation_time){
			probable_creation_time = stat_buf.stx_mtime.tv_sec; // file modification time
		}
		
		compsky::asciify::asciify(itr, probable_creation_time, '\n');
	}
	write(1, buf, compsky::utils::ptrdiff(itr,buf));
}
