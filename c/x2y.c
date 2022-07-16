#include <stdio.h> // for printf
#include <inttypes.h> // for uint64_t
#include "double_from_string.h"


int main(const int argc,  const char* const* argv){
	while(*(++argv) != NULL){
		const double x = double_from_string(*argv);
		printf("%.2f\n",  (x OFFSET) SCALE);
	}
	return 0;
}
