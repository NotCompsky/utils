extern "C" {
# include <lexbor/dom/dom.h>
}
#include <cstdio>
#include <compsky/deasciify/a2n.hpp>
#include "collection.hpp"
#include "element.hpp"
#include "doc.hpp"
#include "parser.hpp"
#include "str_utils.hpp"


typedef unsigned char uchar;


void log(const char* const str){
	printf("%s\n", str);
	fflush(stdout);
}


int main(int argc,  char* const* argv){
	if (argc != 3){
		printf("Usage: [HTML_FILE] [SELECTOR_PATH]\n");
		return 0;
	}
	
	FILE* f = fopen(argv[1], "rb");
	if (f == nullptr)
		return 1;
	
	char html[100 * 1024];
	const size_t html_sz = fread(html,  1,  sizeof(html) - 1,  f);
	if (html_sz == 0)
		return 2;
	
	Parser parser;
	Doc doc(parser, html, html_sz);
	Element element(doc.get_element_from_class_selector_path(argv[2]));
	if (not element.is_null())
		printf("Found element\n");
	else
		printf("No such element found\n");
	
	return 0;
}

template class Collection<1000>;
