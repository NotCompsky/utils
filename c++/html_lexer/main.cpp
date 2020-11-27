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


lxb_dom_element_t* get_element_from_class_selector_path(const Doc& doc,  const char* selector_path){
	/* selector_path being @TAG.CLASS#ATTR=VALUE+INDEX>...>:ATTRIBUTE
	 * e.g. (omitting >:ATTRIBUTE):
	 *   @div>@a                       <div><a></a></div>
	 *   @div.display-box>@img         <div class="display-box"><img/></div>
	 *   .display#onclick>@a          <??? class="display" onclick=???><a></a></???>
	 *   .foo#data-bar=ree           <??? class="foo" data-bar="ree"></???>
	 *   @div>>>@a                     <div><???><???><???><a></a></???></???></???>
	 *   div+2                       <div/><div/><div/>  (3rd div)
	 * e.g. (only ATTRIBUTE):
	 *   (EMPTY) to match inner text content
	 *   href
	 *   src
	 * Only the first element is a genuine global search - all the subsequent levels only filter direct child nodes. WARNING: Currently, it is assumed that a tag_name or class_name is supplied in the top-most level.
	 */
	lxb_dom_element_t* element = doc.document->dom_document.element;
	unsigned level_depth = 0;
	do {
		const char* tag_name   = after_next_char(selector_path, '@');
		const char* class_name = after_next_char(selector_path, '.');
		const char* attr_name  = after_next_char(selector_path, '#');
		const char* attr_val   = after_next_char(selector_path, '=');
		const char* indx_val   = after_next_char(selector_path, '+');
		selector_path = after_next_char(selector_path, '>');
		if (tag_name >= selector_path)
			tag_name = nullptr;
		if (class_name >= selector_path)
			class_name = nullptr;
		if (attr_name >= selector_path)
			attr_name = nullptr;
		if (attr_val >= selector_path)
			attr_val = nullptr;
		unsigned indx = 0;
		if (indx_val < selector_path)
			indx = a2n<unsigned>(indx_val);
		
		if (level_depth == 0)
			element = Element(element).get_element_given_tag_class_attr_indx(tag_name, class_name, attr_name, attr_val, indx);
		else
			element = Element(element).get_direct_child_element_given_tag_class_attr_indx(tag_name, class_name, attr_name, attr_val, indx);
	} while(selector_path != nullptr);
}


int main(int argc,  const char* const* argv){
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
	Collection<16> collection(doc);
	
	if (doc.get_elements_of_class_name(collection, argv[2]))
		return 3;
	
	if (collection.length() == 0){
		printf("Found no elements with that class\n");
		return 1;
	}
	
	Element el(collection, 0);
	
	if (not el.is_null())
		Element(get_element_from_class_selector_path(doc, argv[2])).print();
	
	return 0;
}

template class Collection<1000>;
