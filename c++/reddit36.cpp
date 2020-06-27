/*
MySQL UDF

# Installation:
g++ reddit36.cpp -fPIC -shared -std=c++17 -o reddit36.o
sudo mv reddit36.o "$(mysql_config --plugindir)/"
CREATE FUNCTION reddit36encode RETURNS STRING SONAME 'reddit36.o';
CREATE FUNCTION reddit36decode RETURNS INTEGER SONAME 'reddit36.o';

# Use:
SELECT reddit36encode(INTEGER);
SELECT reddit36decode(STRING);

*/


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <mariadb/mysql.h>
#include <inttypes.h>
#include <string> // for std::char_traits
#include <vector>


struct Str36 {
	char str[13 /* i.e. cieling(log36(UINT64_MAX)) */ + 1];
};
std::vector<Str36> strs;


extern "C" {

bool reddit36encode_init(UDF_INIT* const initid [[unused]],  UDF_ARGS* const args,  char* const msg_buf){
	if (args->arg_count != 1  or  args->arg_type[0] != INT_RESULT){
		constexpr static const char* const msg = "Usage: reddit36encode(integer)";
		memcpy(msg_buf,  msg,  std::char_traits<char>::length(msg));
		return 1;
	}
	return 0;
}

void reddit36encode_deinit(UDF_INIT* const initid){
	strs.clear();
}

char* reddit36encode(UDF_INIT* const initid [[unused]],   const UDF_ARGS* const args,  char* result,  unsigned long& ret_length,  char& is_null,  char* const error [[unused]]){
	static
	constexpr
	char const map[36] = {
		'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
		'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'
	};
	
	Str36& str = strs.emplace_back();
	
	//char buf[11 /* 11 i.e. log64(UINT64_MAX) */ + 1];
	uint64_t n = *((long long*)args->args[0]);
	uint64_t m = n;
	
	is_null = false;
	ret_length = 0;
	
	while (m != 0){
		++ret_length;
		m /= 36;
	}
	
	auto _ret_length = ret_length;
	
	str.str[_ret_length] = 0;
	while (n != 0){
		str.str[--_ret_length] = map[n & 63];
		n /= 36;
	}
	
	return str.str;
}

bool reddit36decode_init(UDF_INIT* const initid [[unused]],  UDF_ARGS* const args,  char* msg_buf){
	if (args->arg_count != 1  or  args->arg_type[0] != STRING_RESULT){
		constexpr const char* const msg = "Usage: reddit36encode(integer)";
		memcpy(msg_buf,  msg,  std::char_traits<char>::length(msg));
		return 1;
	}
	return 0;
}

void reddit36decode_deinit(UDF_INIT* const initid){}

uint64_t reddit36decode(UDF_INIT* const initid [[unused]],  UDF_ARGS* const args,  char* const is_null [[unused]],  char* const error [[unused]]){
	// Not using a map, because: sorry, unimplemented: non-trivial designated initializers not supported
	const char* const arg = args->args[0];
	auto len = args->lengths[0];
	uint64_t n = 0;
	for (auto i = 0;  i < len;  ++i){
		n *= 36;
		const char c = arg[i];
		n  +=  (c >= 'a') * (c <= 'z') * (c - 'a' + 10)  +  (c >= '0') * (c <= '9') * (c - '0');
	}
	return n;
}

} // extern "C"
