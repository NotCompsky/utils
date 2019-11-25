#include <inttypes.h> // for uintptr_t
#include <unistd.h> // for write
#include <string.h> // for strlen
#include "bool.h"

size_t process_string(char* const S){
	bool preceded_by_char = false;
	
	char* s = S;
	while(*s != 0){
		if (*s >= 'a'  &&  *s <= 'z'){
			// lowercase
			if (!preceded_by_char){
				*s += 'A' - 'a';
				preceded_by_char = true;
			}
		} else if (*s >= 'A'  &&  *s <= 'Z'){
			// uppercase
		} else {
			// Not alphabetical
			preceded_by_char = false;
		}
		++s;
	}
	*(s++) = '\n';
	
	write(1,  S,  (uintptr_t)s - (uintptr_t)S);
}

int main(const int argc,  char** argv){
	for (int i = 1;  i < argc;  ++i)
		process_string(argv[i]);
	return 0;
}
