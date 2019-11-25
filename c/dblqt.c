#include <inttypes.h> // for uintptr_t
#include <unistd.h> // for write
#include <string.h> // for strlen

char* process_string(const char* s,  char* buf){
	*(buf++) = '"';
	while(*s != 0){
		if (*s == '"'  ||  *s == '\\')
			*(buf++) = '\\';
		*(buf++) = *(s++);
	}
	*(buf++) = '"';
	*(buf++) = '\n';
	return buf;
}

int main(const int argc,  const char** argv){
	size_t buf_size = 0;
	for (int i = 1;  i < argc; ++i)
		buf_size  +=  1 + strlen(argv[i]) + 1 + 1;
	char buf[2*buf_size];
	char* itr = buf;
	for (int i = 1;  i < argc; ++i)
		itr = process_string(argv[i],  itr);
	write(1,  buf,  (uintptr_t)itr - (uintptr_t)buf);
	return 0;
}
