#include <stdio.h>


int main(const int argc,  const char** argv){
	size_t n = 0;
	int j = 0;
	for (const char* it = argv[1];  *it != 0;  ++it){
		n |= (*it - '0') << j++;
	}
	printf("%lu\n", n);
}
