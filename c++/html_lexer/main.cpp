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
	if (argc != 4){
		printf(
			"USAGE\n"
			"    HTML_FILE SELECTOR_PATH ATTRIBUTE\n"
			"OPTIONS\n"
			"    ATTRIBUTE\n"
			"        Value of . gets the node's inner text\n"
			"        If the element is found but has no such attribute, NULL is returned\n"
			"EXAMPLES\n"
			"    /tmp/html @div#profile-img\n"
			"    /tmp/html @div#profile-img>@img src\n"
		);
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
	if (not element.is_null()){
		const std::string_view v = element.get_value(argv[3]);
		printf("%.*s\n", (int)v.size(), v.data());
	} else
		printf("No such element found\n");
	
	return 0;
}

template class Collection<1000>;
