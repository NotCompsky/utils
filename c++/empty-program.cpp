#include <unistd.h>
#include <cstdio>
#include <string> // for char_traits

int main(){
	constexpr const char* msg = "Press ENTER to stop: ";
	write(1, msg, std::char_traits<char>::length(msg));
	getchar();
	return 0;
}
