#include <iostream>

void print_bits(unsigned char c){
	for (uint_fast8_t i=0; i<8; ++i){
		std::cout << (c & 1);
		c = c >> 1;
	}
}

int main(const int argc, unsigned char* argv[]){
	unsigned char* it;
	for (uint_fast8_t i=1; i<argc; ++i)
		for (it=argv[i]; *it!=0; ++it){
			std::cout << +(*it) << "\t";
			print_bits(*it);
			std::cout << std::endl;
		}
}
