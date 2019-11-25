#include <compsky/asciify/print.hpp>
#include <boost/regex.hpp>


boost::match_results<const char*> what;


namespace _f {
	constexpr static const compsky::asciify::flag::StrLen strlen;
}


FILE* f;


namespace cl {
	constexpr static const char* const PURPLE			  = "\033[0;0;95m";
	constexpr static const char* const CYAN				= "\033[0;0;96m";
	constexpr static const char* const DCYAN			   = "\033[0;0;36m";
	constexpr static const char* const GREEN			   = "\033[0;0;92m";
	constexpr static const char* const YELLOW			  = "\033[0;0;93m";
	constexpr static const char* const RED				 = "\033[0;0;91m";

	constexpr static const char* const BOLD				= "\033[0;0;1m";
	constexpr static const char* const UNDERLINED		  = "\033[0;0;4m";
	constexpr static const char* const END				 = "\033[0;0;0m";

	constexpr static const char* const WHITE			   = "\033[1;37m";
	constexpr static const char* const BLUE				= "\033[0;34m";
	constexpr static const char* const LBLUE			   = "\033[1;34m";
	constexpr static const char* const LGREEN			  = "\033[1;32m";
	constexpr static const char* const LCYAN			   = "\033[1;36;40m";
	constexpr static const char* const LRED				= "\033[1;31m";
	constexpr static const char* const LPURPLE			 = "\033[1;35m";
	constexpr static const char* const ORANGE			  = "\033[0;33;40m";
	constexpr static const char* const BROWN			   = "\033[0;0;33m";
	constexpr static const char* const GREY				= "\033[0;0;37m";
	constexpr static const char* const LGREY			   = "\033[0;0;37m";
}


void get_ij(const char* s,  const char* const end,  unsigned int& i,  unsigned int& j){
	i = 0;
	j = 0;
	while(s != end){
		if (*s == '\n'){
			++j;
			i = 0;
		} else ++i;
		++s;
	}
}


int main(const int argc,  const char** const argv){
	if (argc == 1){
		constexpr static const char* const _usage =
			"Usage:\n"
			"boost-regex REGEX_FILE FILE1 [FILE2 FILE3 ...]\n"
			"Where FILE<N> contain contents to be matched against the regex.\n"
		;
		fwrite(
			_usage,
			1,
			std::char_traits<char>::length(_usage), // C++17
			stderr
		);
		return 0;
	}
	
	char asciify_buf[4096];
	char* asciify_itr = asciify_buf;
	
	boost::basic_regex<char, boost::cpp_regex_traits<char>>* regexpr;
	
	for (size_t i = 1;  i < argc;  ++i){
		const char* const fp = argv[i];
		if(unlikely(!(f = fopen(fp, "rb")))){
			compsky::asciify::write(asciify_buf, stderr, "Cannot open file: ", fp, '\n');
			return 3;
		}
		
		fseek(f, 0, SEEK_END);
		const size_t fsize = ftell(f);
		fseek(f, 0, SEEK_SET);
		
		char* buf = (char*)malloc(fsize + 1);
		if (unlikely(buf == nullptr))
			return 4096;
		
		fread(buf, 1, fsize, f);
		fclose(f);
		
		if(i == 1){
			regexpr = new boost::basic_regex<char, boost::cpp_regex_traits<char>>(buf,  boost::regex::perl | boost::regex::optimize);
			continue;
		}
		
		const bool b = boost::regex_search(const_cast<const char*>(buf),  const_cast<const char*>(buf) + fsize,  what,  *regexpr);
		
		compsky::asciify::asciify(asciify_itr,
			_f::strlen, buf, fsize, '\n',
			 cl::PURPLE,  "Matched: ",  b, '\n'
		);
		
		if (!b){
			continue;
		}
		
		for (size_t j = 1;  j < what.size();  ++j){
			compsky::asciify::asciify(asciify_itr,
				cl::BLUE, "Group ", j, ": ", what[j].matched
			);
			
			unsigned int x;
			unsigned int y;
			get_ij(buf, what[j].first, x, y);
			
			if (what[j].matched){
				compsky::asciify::asciify(asciify_itr,
					cl::BROWN, '\t', '@', cl::END, '(', cl::GREEN, x, cl::END, ", ", cl::GREEN, y,  cl::END, ')', '\n',
					cl::END,
					_f::strlen,
					what[j].first,
					what[j].second - what[j].first,
					'\n'
				);
			} else compsky::asciify::asciify(asciify_itr, '\n');
		}
	}
	compsky::asciify::asciify(asciify_itr, cl::END);
	fwrite(asciify_buf,  1,  (uintptr_t)asciify_itr - (uintptr_t)asciify_buf,  stdout);
}
