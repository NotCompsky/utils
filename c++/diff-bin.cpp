#include <iostream>
#include <sys/stat.h> // for stat


namespace cl {
	namespace fg {
		constexpr static const char* const BLACK		   = "\033[0;30m";
		constexpr static const char* const BLUE			= "\033[0;34m";
		constexpr static const char* const GREEN		   = "\033[0;32m";
		constexpr static const char* const CYAN			= "\033[0;36m";
		constexpr static const char* const RED			 = "\033[0;31m";
		constexpr static const char* const PURPLE		  = "\033[0;35m";
		constexpr static const char* const YELLOW		  = "\033[0;33m";
		constexpr static const char* const LIGHT_GREY	  = "\033[0;37m";
		
		constexpr static const char* const DARK_GREY	   = "\033[1;30m";
		constexpr static const char* const LIGHT_BLUE	  = "\033[1;34m";
		constexpr static const char* const LIGHT_GREEN	 = "\033[1;32m";
		constexpr static const char* const LIGHT_CYAN	  = "\033[1;36m";
		constexpr static const char* const LIGHT_RED	   = "\033[1;31m";
		constexpr static const char* const LIGHT_PURPLE	= "\033[1;35m";
		constexpr static const char* const LIGHT_YELLOW	= "\033[1;33m";
		constexpr static const char* const WHITE		   = "\033[1;37m";
	}
	namespace bg {
		constexpr static const char* const BLACK		   = "\033[0;40m";
		constexpr static const char* const BLUE			= "\033[0;44m";
		constexpr static const char* const GREEN		   = "\033[0;42m";
		constexpr static const char* const CYAN			= "\033[0;46m";
		constexpr static const char* const RED			 = "\033[0;41m";
		constexpr static const char* const PURPLE		  = "\033[0;45m";
		constexpr static const char* const YELLOW		  = "\033[0;43m";
		constexpr static const char* const LIGHT_GREY	  = "\033[0;47m";
		
		constexpr static const char* const DARK_GREY	   = "\033[1;40m";
		constexpr static const char* const LIGHT_BLUE	  = "\033[1;44m";
		constexpr static const char* const LIGHT_GREEN	 = "\033[1;42m";
		constexpr static const char* const LIGHT_CYAN	  = "\033[1;46m";
		constexpr static const char* const LIGHT_RED	   = "\033[1;41m";
		constexpr static const char* const LIGHT_PURPLE	= "\033[1;45m";
		constexpr static const char* const LIGHT_YELLOW	= "\033[1;43m";
		constexpr static const char* const WHITE		   = "\033[1;47m";
	}
	constexpr static const char* const DEFAULT			= "\033[0m";
	constexpr static const char* const UNDERLINE		  = "\033[4m";
	constexpr static const char* const BLINK			  = "\033[5m";
	constexpr static const char* const INVERSE			= "\033[7m";
	constexpr static const char* const CONCEALED		  = "\033[8m";
}


constexpr
static const char* const colours1[] = {
	cl::fg::BLUE,
	cl::fg::GREEN,
	cl::fg::CYAN,
	cl::fg::RED,
	cl::fg::PURPLE,
	cl::fg::YELLOW,
	cl::fg::LIGHT_GREY
};

constexpr
static const char* const colours2[] = {
	cl::fg::LIGHT_BLUE,
	cl::fg::LIGHT_GREEN,
	cl::fg::LIGHT_CYAN,
	cl::fg::LIGHT_RED,
	cl::fg::LIGHT_PURPLE,
	cl::fg::LIGHT_YELLOW,
	cl::fg::WHITE
};


void print_bits(unsigned char c){
	for (uint_fast8_t i=0; i<8; ++i){
		std::cout << (c & 1);
		c /= 2;
	}
}

void cmp(unsigned char c){
	std::cout << +c << "\t";
	print_bits(c);
	std::cout << "\t(";

	if (c == '\n')
		std::cout << "\\n";
	else if (c == '\r')
		std::cout << "\\r";
	else
		std::cout << c;

	std::cout << ")\tfrom ";
}

int main(const int argc, char* argv[]){
	FILE* ifiles[argc -1];
	struct stat stat_buf;
	uint_fast64_t nbytes = 0;
	for (uint_fast8_t i=1; i<argc; ++i){
		if (stat(argv[i], &stat_buf) == -1){
			throw std::runtime_error("Cannot stat file");
		} else {
			if ((nbytes) && (stat_buf.st_size != nbytes)){
				std::cout << "W: File sizes do not match" << std::endl;
			}
			if ((!nbytes) || stat_buf.st_size <= nbytes)
				nbytes = stat_buf.st_size;
		}
		
		ifiles[i-1] = fopen(argv[i], "r");
	}
	unsigned char c;
	unsigned char C;
	uint_fast64_t consec_eq = 0;
	uint_fast64_t consec_df = 0;
	uint_fast64_t i=0;
	for (i=0; i<nbytes; ++i){
		c = getc(ifiles[0]);
		for (uint_fast8_t j=1; j<argc-1; ++j){
			C = getc(ifiles[j]);
			if (C != c){
				if (consec_eq != 0){
					std::cout << "Position " << +i << "\tRun of " << +consec_eq << " successes" << std::endl;
					consec_eq = 0;
				}
				++consec_df;
				std::cout << colours2[j-1];
				cmp(c);
				std::cout << cl::DEFAULT;
				std::cout << colours1[j-1] << argv[j] << cl::DEFAULT << std::endl;
				
				std::cout << colours2[j];
				cmp(C);
				std::cout << cl::DEFAULT;
				std::cout << colours1[j] << argv[j+1] << cl::DEFAULT << std::endl;
			} else {
				if (consec_eq++ == 0){
					std::cout << "Position " << +i << "\tRun of " << +consec_df << " failures" << std::endl;
					consec_df = 0;
				}
			}
		}
	}
	std::cout << "Position " << +i << "\tRun of " << +consec_eq << " successes" << std::endl;
}
