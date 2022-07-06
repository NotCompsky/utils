#include <compsky/mimetype/metadata.hpp>
#include <compsky/os/read.hpp>
#include <cstdio>


int main(const int argc,  const char* const* const argv){
	char buf[29];
	
	for (int i = 1;  i < argc;  ++i){
		compsky::os::ReadOnlyFile file(argv[i]);
		if (unlikely(file.is_null() or (file.size<0>() < sizeof(buf)))){
			printf(" doesnt exist");
			return 1;
		}
		
		unsigned w = 0;
		unsigned h = 0;
		file.read_into_buf(buf, sizeof(buf));
		compsky::mimetyp::get_media_dimensions_from_buf(buf, w, h);
		printf("%u %u %s %s\n", w, h, compsky::mimetyp::mimetype2str(buf), argv[i]);
	}
}
