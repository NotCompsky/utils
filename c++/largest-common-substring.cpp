#include <cstdio>
#include <cstdint>
#include <cstring>

template<typename... Args>
constexpr uint32_t min(Args&&... args, const uint32_t n){
	return min(min(args...), n);
}
constexpr uint32_t min(const uint32_t a,  const uint32_t b){
	return (a > b) ? b : a;
}

uint64_t longest_common_substr(const char* str1,  const uint32_t str1_len,  const char* str2,  const uint32_t str2_len,  const uint32_t min_length){
	uint32_t current_max_length = min_length;
	uint32_t offset1_of_largest_common_substr = 0;
	
	for (uint32_t i = 0;  i < str1_len;  ++i){
		for (uint32_t j = 0;  j < str2_len;  ++j){
			uint32_t x = 0;
			uint32_t y = min(str1_len-i, str2_len-j);
			while ((x < y) and (str1[i + x] == str2[j + x])){
				++x;
			}
			if (x > current_max_length){
				current_max_length = x;
				offset1_of_largest_common_substr = i;
			}
		}
	}
	return ((uint64_t)offset1_of_largest_common_substr << 32) | current_max_length;
}

void print_longest_common_substr(const char* str1,  const uint32_t str1_len,  const char* str2,  const uint32_t str2_len){
	const uint64_t n = longest_common_substr(str1, str1_len, str2, str2_len, 0);
	printf("%.*s\n", (int)(n&0xffffffff), str1+((n>>32)&0xffffffff));
}

int main(const int argc,  const char* const* const argv){
	print_longest_common_substr(argv[1], strlen(argv[1]), argv[2], strlen(argv[2]));
}
