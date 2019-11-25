#include <stdio.h>
#include <inttypes.h>


uint64_t ip_to_integer(const char* str){
	uint8_t ints[4] = {0, 0, 0, 0};
	uint8_t* ints_itr = ints;
	do {
		if (*str == '.'){
			++ints_itr;
			continue;
		}
		*ints_itr *= 10;
		*ints_itr += *str - '0';
	} while(*(++str) != 0);
	
	return 
		(ints[0] << 24) |
		(ints[1] << 16) |
		(ints[2] << 8)  |
		(ints[3] << 0)
	;
}


int main(const int argc,  const char* const* argv){
	++argv;
	do {
		printf("%lu\n", ip_to_integer(*argv));
	} while(*(++argv) != NULL);
}
