/*
MySQL UDF

# Installation:
g++ file-exists.cpp -fPIC -shared -std=c++17 -o file-exists.o
sudo mv file-exists.o "$(mysql_config --plugindir)/"
CREATE FUNCTION fileexists RETURNS INT SONAME 'file-exists.o';

# Use:
SELECT fileexists(STRING);

# WARNING: If protected by AppArmor or similar, might need to grant mysql read permissions - e.g. "/** r," to "/etc/apparmor.d/usr.sbin.mysqld/abstractions/mysql" - then restart AppArmor "/etc/init.d/apparmor restart"

*/


#include <stdlib.h>
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

bool fileexists_init(UDF_INIT* const,  UDF_ARGS* const args,  char* msg_buf){
	if (args->arg_count != 1  or  args->arg_type[0] != STRING_RESULT){
		constexpr const char* const msg = "Usage: fileexists(integer)";
		memcpy(msg_buf,  msg,  std::char_traits<char>::length(msg));
		return 1;
	}
	return 0;
}

void fileexists_deinit(UDF_INIT* const initid){}

int fileexists(UDF_INIT* const,  UDF_ARGS* const args,  char* const is_null,  char* const){
	char buf[4096];
	const char* const arg = args->args[0];
	const auto len = args->lengths[0];
	memcpy(buf, arg, len);
	buf[len] = 0;
	FILE* const log = fopen("/tmp/file-exists.log", "rb");
	if (log != nullptr){
		fprintf(log, "buf = %s\n", buf);
		fprintf(log, "arg = %s\n", arg);
	}
	FILE* const f = fopen(buf, "rb");
	return (f != nullptr);
}

} // extern "C"
