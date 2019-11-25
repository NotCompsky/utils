#include <inttypes.h> // for uint64_t
#include <cstring> // for malloc
#include <cstdio> // for printf


bool is_cached(const uint64_t* ids,  const size_t ids_len,  const size_t ids_len_log2,  const uint64_t id){
	// Inspired by Matt Pulver's 2011 article: http://eigenjoy.com/2011/01/21/worlds-fastest-binary-search/
	size_t i = 0;
	for(size_t b = 1 << ids_len_log2;   b != 0;   b >>= 1){
		// TODO: Check for endianness?
		size_t j = i | b;
		if (j >= ids_len)
			;
		else if (ids[j] <= id)
			i = j;
	}
	return (ids[i] == id);
}

constexpr
size_t log2(size_t m){
	size_t n = 0;
	while (m >>= 1)
		++n;
	return n;
}

constexpr
uint64_t atoi(const char* s){
	uint64_t n = 0;
	while(*s != 0){
		n *= 10;
		n += *s - '0';
		++s;
	}
	return n;
}


constexpr static const uint64_t primes[] = {
# include "primes_10k.txt"
};

constexpr static const size_t n_primes = sizeof(primes) / sizeof(uint64_t);
constexpr static const size_t n_primes_log2 = log2(n_primes);


int main(const int argc,  const char** const argv){
	for (auto i = 1;  i < argc;  ++i)
		printf("%s\n", (is_cached(primes, n_primes, n_primes_log2, atoi(argv[i]))) ? "True" : "False");
}
