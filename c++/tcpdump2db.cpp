#include <cstdio> // for printf
#include <inttypes.h>
#include <cstdlib> // for free
#include <string> // for std::char_traits
#ifdef TESTS
# include <cassert>
#endif
//#include <compsky/mysql/query.hpp>


#ifndef DEBUG
# undef printf
# define printf(...)
#endif


/*
 * Expects STDIN input of format akin to
 * 14:32:58.078864 IP pc-111-234-74.magd.cam.ac.uk.46137 > styx-234-62.magd.cam.ac.uk.domain: 49423+ [1au] A? play.google.com. (44)
 */


/*namespace _mysql {
	MYSQL* obj;
	constexpr static const size_t auth_sz = 512;
	char auth[auth_sz];
}


namespace _f {
	constexpr static compsky::asciify::flag::Escape esc;
}*/


uint64_t ip_to_integer(const char* str){
	constexpr static const uint8_t top_level_ip = 0;
	uint8_t ints[6] = {top_level_ip, 0, 0, 0, 0, 1};
	uint8_t* ints_itr = ints + 1;
	while(true){
		if (*str == '-'  ||  *str == '.'){
			if (*(++ints_itr) != 0)
				break;
		} else if (*str >= '0'  &&  *str <= '9'){
			*ints_itr *= 10;
			*ints_itr += *str - '0';
		} else {
			break;
		}
		++str;
	}
	
	ints_itr = (ints[3] == 0) ? ints : ints + 1;
	// If we only recieved 3 IP things (e.g. 123.45.123), then make ints_itr[0] equal to zero, to avoid the result being bit-shifted too much.
	
	return 
		(ints_itr[0] << 24) |
		(ints_itr[1] << 16) |
		(ints_itr[2] << 8)  |
		(ints_itr[3] << 0)
	;
}


uint64_t get_timestamp(const char* str){
	uint64_t n = 0;
	
	n += *(++str) - '0';
	n *= 10;
	n += *(++str) - '0';
	
	n *= 6;
	++str; // Skip :
	
	n += *(++str) - '0';
	n *= 10;
	n += *(++str) - '0';
	
	n *= 6;
	++str; // Skip :
	
	n += *(++str) - '0';
	n *= 10;
	n += *(++str) - '0';
	
#ifdef TESTS
	assert(str[1] == '.');
#endif
	
	return n;
}


void jump_to_end_of_substr(const char c,  char*& str){
	while(*str != c)
		++str;
	++str;
}


bool reverse_match(const char* end_of_str,  const char* s){
	while(*s != 0){
		if (*s != *end_of_str)
			break;
		++s;
		--end_of_str;
	}
	return !(*s);
}


const char* go_to_previous_dot(const char* s){
	while(*(--s) != '.');
	return s;
}


bool startswith(const char* a,  const char* b){
	while(*b != 0){
		if (*a != *b)
			return false;
		++a;
		++b;
	}
	return true;
}


const char* get_important_domain_name(const char* start,  const char* end){
	/*
	 * e.g.
	 *  www.google.com -> google.com
	 *  www.ox.ac.uk -> ox.ac.uk
	 */
	
	if (startswith(start, "www."))
		start += 4;
	
	if (reverse_match(end, "moc.oedivelgoog."))
		return "googlevideo.com";
	
	if (reverse_match(end, "moc.gmity."))
		return "ytimg.com";
	
	return start;
}


void jump_to_end_of_substr(char*& str,  const char c){
	while(*str != c)
		++str;
}


void jump_to_end_of_substr(char*& a,  const char* const b){
	const char* B = b;
	while(*a != 0  &&  *B != 0){
		if(*a != *B){
			B = b;
		} else {
			++B;
		}
		++a;
	}
}


bool string_doesnt_continue(const char* const str,  const char* const msg,  const char* const str_orig){
	if (*str == 0){
		// Format does not match a DNS request
		fprintf(stderr,  "*str==0 after %s: %s\n",  msg,  str_orig);
		return true;
	}
	return false;
}


void process_line(char* str){
	const char* const str_orig = str;
	
	//const uint64_t timestamp = get_timestamp(str);
	
	/* Doesnt seem to work: jump_to_end_of_substr(str, " IP ");
	if (string_doesnt_continue(str, "IP", str_orig))
		return;
	++str;*/
	str += std::char_traits<char>::length("23:52:45.114544 IP ");
	if (startswith(str, "pc-"))
		str += 3;
	
	const uint64_t ip_sub_addr = ip_to_integer(str);
	if (ip_sub_addr >> 16  ==  0){
		// i.e. styx-123-456.magd.cam.ac.uk.domain (2 IP sections rather than 3)
		fprintf(stderr,  "Bad IP address\n");
		return;
	}
	
	while(*str != '?'  &&  *str != 0)
		++str;
	if (string_doesnt_continue(str, "?", str_orig))
		return;
	
	str += std::char_traits<char>::length("? ");
	const char* const domain = str;
	
	jump_to_end_of_substr(str, ' ');
	if (string_doesnt_continue(str, "<SPACE>", str_orig))
		return;
	++str;
	*(str - 2) = 0; // Overwrite trailing period
	
	char* const domain_end = str - 3;
	
	if (reverse_match(domain_end, "ku.ca.mac.dgam." /* .magd.cam.ac.uk */))
		// NOTE: str is guaranteed to be longer than ".magd.cam.ac.uk"
		return;
	
	if (reverse_match(domain_end, "lacol" /* .local or (NULL)local */))
		return;
	
	const char* const important_domain = get_important_domain_name(domain, domain_end);
	
	/*compsky::mysql::exec(
		_mysql::obj,
		"INSERT INTO dns "
		"(ip, domain)"
		"VALUES"
		"(",
			ip_sub_addr, ',',
			"(SELECT id FROM domain WHERE name=\"", _f::esc, '"', important_domain, "\")"
		")"
	);*/
	
	printf("%lu\t%s \n	%s\n", ip_sub_addr, important_domain, str_orig);
}


int main(const int argc,  const char* const* argv){
	/*
	 * USAGE
	 *  tcpdump2db [MySQL_Config_File_Path]
	 */
	
	const char* const mysql_cfg_fp = argv[1];
	
	//compsky::mysql::init(_mysql::obj, _mysql::auth, _mysql::auth_sz, mysql_cfg_fp);
	
	char* line = nullptr;
	size_t line_sz = 0;
	while(getline(&line, &line_sz, stdin) != -1){
		printf("\n@@@ %s", line);
		process_line(line);
	}
	free(line);
	
	//compsky::mysql::wipe_auth(_mysql::auth, _mysql::auth_sz);
}
