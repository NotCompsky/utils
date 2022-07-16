double double_from_string(const char* str){
	const char* s = str;
	unsigned int x[2] = {0, 0};
	unsigned int* y = x;
	unsigned int ratio = 10;
	while(*s != 0){
		const char c = *s;
		switch(c){
			case '0' ... '9':
				*y *= 10;
				*y += c - '0';
				break;
#ifdef ALLOW_TWELVES
			case '\'':
				// E.g. 6'2 (6ft 2)
				ratio = 12;
#endif
			case '.':
			default:
				str = s;
				++y;
				break;
		}
		++s;
	}
	
	if (x[1] == 0)
		// No period in input
		return x[0];
	
	unsigned int digits_after_period = (uintptr_t)s - (uintptr_t)str + 1 - 1 - 1;
	uint64_t multiplier = ratio;
	while(--digits_after_period != 0){
		multiplier *= ratio;
	}
	
	x[0] *= multiplier;
	x[0] += x[1];
	return (double)x[0] / (double)multiplier;
}
