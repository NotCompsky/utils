#include <stdio.h>
#include <unistd.h> // for STD(IN|OUT)_FILENO


size_t my_atoi(const char* str){
	size_t n = 0;
	char* s = (char*)str;
	while (*s != 0){
		n *= 10;
		n += *s - '0';
		++s;
	}
	return n;
}

int main(const int argc,  const char** argv){
	char bits[65];
	bits[64] = '\n';
	
	for (auto j = 1;  j < argc;  ++j){
		size_t n = my_atoi(argv[j]);
		for (char i = 0;  i < 64;  ++i){
			bits[i] = '0' + (n & 1);
			n /= 2;
		}
		if (write(STDOUT_FILENO, bits, 65) != 65)
			return 1;
	}
	return 0;
}
