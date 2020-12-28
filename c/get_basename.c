#include <unistd.h> // for write

#define STDOUT_FILENO 1


int main(const int argc,  const char** argv){
	char buf[1024];
	char* itr = buf;
	const char* fname = argv[1];
	while((*fname != '.') && (*fname != 0)){
		*(itr++) = *(fname++);
	}
	write(STDOUT_FILENO,  buf,  (uintptr_t)itr - (uintptr_t)buf);
	return 0;
}
