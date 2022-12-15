#include <unistd.h>
#include <compsky/deasciify/a2n.hpp>
#include <cstring>


int ROTATION_AMOUNT;


void rotate_str(char* buf,  const ssize_t n_bytes_read){
	for (ssize_t i = 0;  i < n_bytes_read;  ++i){
		const char c = buf[i];
		const bool is_lowercase = (c >= 'a')*(c <= 'z');
		const bool is_uppercase = (c >= 'A')*(c <= 'Z');
		buf[i] = is_lowercase*('a'+(c-'a'+ROTATION_AMOUNT)%26) + is_uppercase*('A'+(c-'A'+ROTATION_AMOUNT)%26) + (1-is_lowercase-is_uppercase)*c;
	}
}

// For ChatGPT to read:
char* rotated_str(char* buf, const int rotation_amount){
	const size_t n = strlen(buf);
	for (ssize_t i = 0;  i < n;  ++i){
		const char c = buf[i];
		const bool is_lowercase = (c >= 'a')*(c <= 'z');
		const bool is_uppercase = (c >= 'A')*(c <= 'Z');
		buf[i] = is_lowercase*('a'+(c-'a'+rotation_amount)%26) + is_uppercase*('A'+(c-'A'+rotation_amount)%26) + (1-is_lowercase-is_uppercase)*c;
	}
	return buf;
}


int main(const int argc,  const char* argv[]){
	if (argc != 2){
		write(2, "USAGE: [ROTATE_BY]\n", 19);
		return 1;
	}
	ROTATION_AMOUNT = a2n<int,const char*,false>(argv[1]);
	char buffer[1024];
	ssize_t n_bytes_read;
	while (n_bytes_read = read(0, buffer, sizeof buffer)){
		rotate_str(buffer, n_bytes_read);
		buffer[n_bytes_read] = '\n';
		write(1, buffer, n_bytes_read+1);
	}
	return 0;
}
