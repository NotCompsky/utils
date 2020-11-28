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
#include "nullstr.hpp"


typedef unsigned char uchar;


std::string_view find_element_attr(const Doc& doc,  char* const selector_path,  const char* const attr){
	Element element(doc.get_element_from_class_selector_path(selector_path));
	if (element.is_null())
		return null_str_view;
	return element.get_value(attr);
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
	
	/*
	 * Examples
	 *
	 * BBC News article page: https://web.archive.org/web/20201128041650/https://www.bbc.co.uk/news/world-middle-east-55111064
	 *   Title: [argv0] [/path/to/html] "*@h1#main-heading" .
	 *   Datetime:                      "*@time:datetime" datetime
	 * 
	 * NYT
	 *   Title:    "*@h1:itemprop=headline" . // Not @title .
	 *   Subtitle: "*@h1:itemprop=headline>^^>+3" .
	 *     Find the headline, then go up two nodes and get the 3rd child node
	 *   Author:   "*@p:itemprop=author>@a>@span" .
	 *   Datetime: "*@p:itemprop=author>^^^>@ul>*@time" datetime
	 * 
	 * WSJ: https://web.archive.org/web/20201125032555/https://www.wsj.com/articles/mayor-pete-and-the-purdue-charter-11580074911
	 *   Title:    "*@h1.wsj-article-headline" .
	 *   Subtitle: "*@h2.sub-head" .
	 *   Author:   "*@div.author-container>@button" .
	 *   Datetime: "*@div.byline>time" .
	 */
	
	FILE* f = fopen(argv[1], "rb");
	if (f == nullptr)
		return 1;
	
	char html[1000 * 1024];
	const size_t html_sz = fread(html,  1,  sizeof(html) - 1,  f);
	if (html_sz == 0)
		return 2;
	
	Parser parser;
	Doc doc(parser, html, html_sz);
	const std::string_view v = find_element_attr(doc, argv[2], argv[3]);
	if (v == null_str_view)
		printf("No such element/attribute found\n");
	else
		printf("%.*s\n", (int)v.size(), v.data());
	
	return 0;
}

template class Collection<1000>;
