#include <compsky/macros/str2switch.hpp>


int main(const int argc,  const char* const* const argv){
	const char* str = argv[1] - 1; // Cancel out the first pre-increment of s. // I'd use BOOST_PP_IF, but I can't be bothered to parse 10 pages of misleading errors
	STR2SWITCH(7,"foobar\0",return 1;);
	return 0;
}
