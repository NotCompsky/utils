#include <stdio.h>
#include <inttypes.h> // for uint64_t


/* From Wikipedia */
struct xorshift64_state {
	uint64_t a;
};
uint64_t xorshift64(struct xorshift64_state* state){
	uint64_t x = state->a;
	x ^= x << 13;
	x ^= x >> 7;
	x ^= x << 17;
	return state->a = x;
}


bool coin_toss(struct xorshift64_state* state,  const char c){
	return (xorshift64(state) & 1);
}


char change_state(const char c){
	constexpr static const char n = 'A' - 'a';
	if ((c >= 'a') && (c <= 'z'))
		return c + n;
	else if ((c >= 'A') && (c <= 'Z'))
		return c - n;
	else return c;
}


void process_string(struct xorshift64_state* state,  char* s){
	while(*s != 0){
		if (coin_toss(state, *s))
			*s = change_state(*s);
		++s;
	}
}


int main(const int argc,  char** argv){
	static xorshift64_state state;
	state.a = 23984234729;
	while(*(++argv) != nullptr){
		process_string(&state, *argv);
		printf("%s\n", *argv);
	}
	return 0;
}
