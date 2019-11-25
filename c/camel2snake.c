#include <unistd.h> // for write
#include <string.h> // for strlen

size_t process_string(const char* s,  char* buf,  size_t buf_indx){
	if ('a' <= *s  &&  *s <= 'z')
		buf[buf_indx++] = *s + 'A' - 'a';
	else
		buf[buf_indx++] = *s;
	
	for (const char* itr = s + 1;  *itr != 0;  ++itr){
		if ('a' <= *itr  &&  *itr <= 'z')
			buf[buf_indx++] = *itr + 'A' - 'a';
		else {
			buf[buf_indx++] = '_';
			buf[buf_indx++] = *itr;
		}
	}
	return buf_indx;
}

int main(const int argc,  const char** argv){
	size_t buf_size = 0;
	for (auto i = 1;  i < argc; ++i)
		buf_size  +=  1 + strlen(argv[i]);
	char buf[2*buf_size];
	size_t buf_indx = 0;
	for (auto i = 1;  i < argc; ++i)
		buf_indx = process_string(argv[i], buf, buf_indx);
	write(1, buf, buf_indx);
	return 0;
}
