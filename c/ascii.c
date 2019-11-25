#include <stdio.h>
#include <stdlib.h>
#include <string.h> // for strlen


int main(const int argc,  const char **argv){
	for (auto i = 1;  i < argc;  ++i){
		const char* s = argv[i];
		size_t len = strlen(s);
		printf("Input string %s of length %lu:",  s,  len);
		for (int j=0; j<len; j++)
			printf("  %d", (int)s[j]);
		printf("\n");
	}
}

