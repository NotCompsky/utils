/*
MySQL UDF

# Installation:
g++ web64.cpp -fPIC -shared -std=c++17 -o web64.o
sudo mv web64.o "$(mysql_config --plugindir)/"
CREATE FUNCTION web64encode RETURNS STRING SONAME 'web64.o';
CREATE FUNCTION web64decode RETURNS INTEGER SONAME 'web64.o';

# Use:
SELECT web64encode(INTEGER);
SELECT web64decode(STRING);

*/


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <mariadb/mysql.h>
#include <inttypes.h>
#include <string> // for std::char_traits
#include <vector>


struct Str64 {
	char str[11 /* 11 i.e. log64(UINT64_MAX) */ + 1];
};
std::vector<Str64> strs;


extern "C" {

bool web64encode_init(UDF_INIT* const initid [[unused]],  UDF_ARGS* const args,  char* const msg_buf){
	if (args->arg_count != 1  or  args->arg_type[0] != INT_RESULT){
		constexpr static const char* const msg = "Usage: web64encode(integer)";
		memcpy(msg_buf,  msg,  std::char_traits<char>::length(msg));
		return 1;
	}
	return 0;
}

void web64encode_deinit(UDF_INIT* const initid){
	strs.clear();
}

char* web64encode(UDF_INIT* const initid [[unused]],   const UDF_ARGS* const args,  char* result,  unsigned long& ret_length,  char& is_null,  char* const error [[unused]]){
	static
	constexpr
	char const map[64] = {
		'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
		'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
		'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '-', '_'
	};
	
	Str64& str = strs.emplace_back();
	
	//char buf[11 /* 11 i.e. log64(UINT64_MAX) */ + 1];
	uint64_t n = *((long long*)args->args[0]);
	uint64_t m = n;
	
	is_null = false;
	ret_length = 0;
	
	while (m != 0){
		++ret_length;
		m /= 64;
	}
	
	auto _ret_length = ret_length;
	
	str.str[_ret_length] = 0;
	while (n != 0){
		str.str[--_ret_length] = map[n & 63];
		n /= 64;
	}
	
	return str.str;
}

bool web64decode_init(UDF_INIT* const initid [[unused]],  UDF_ARGS* const args,  char* msg_buf){
	if (args->arg_count != 1  or  args->arg_type[0] != STRING_RESULT){
		constexpr const char* const msg = "Usage: web64encode(integer)";
		memcpy(msg_buf,  msg,  std::char_traits<char>::length(msg));
		return 1;
	}
	return 0;
}

void web64decode_deinit(UDF_INIT* const initid){}

uint64_t web64decode(UDF_INIT* const initid [[unused]],  UDF_ARGS* const args,  char* const is_null [[unused]],  char* const error [[unused]]){
	// Not using a map, because: sorry, unimplemented: non-trivial designated initializers not supported
	const char* const arg = args->args[0];
	auto len = args->lengths[0];
	uint64_t n = 0;
	for (auto i = 0;  i < len;  ++i){
		n *= 64;
		const char c = arg[i];
		n  +=  (c >= 'A') * (c <= 'Z') * (c - 'A')  +  (c >= 'a') * (c <= 'z') * (26 + c - 'a')  +  (c >= '0') * (c <= '9') * (26 + 26 + c - '0')  +  (c == '-') * 62  +  (c == '_') * 63; // TODO: Maybe could be |=
	}
	return n;
}

} // extern "C"
