#pragma once


inline
const char* after_next_char(const char* str,  const char c){
	while(true){
		if (*str == 0)
			return nullptr;
		if (*str == c)
			return str;
		++str;
	}
}

inline
bool streq(const char* a,  const char* b){
	if (a == nullptr)
		return true;
	while(true){
		if (*a != *b)
			return false;
		if (*a == 0)
			return true;
		++a;
		++b;
	}
}


inline
bool streq(const char* a,  const char* b,  unsigned length_to_compare){
	if (a == nullptr)
		return true;
	while(length_to_compare){
		if (*a != *b)
			return false;
		++a;
		++b;
		--length_to_compare;
	}
	return true;
}
