extern "C" {
# include <lexbor/dom/dom.h>
# include <lexbor/html/html.h>
}
#include <cstdio>
#include <stdexcept>


void assert_not_null(void* x){
	if (x==nullptr)
		throw std::runtime_error("Unexpected nullptr");
}


typedef unsigned char uchar;


void log(const char* const str){
	printf("%s\n", str);
	fflush(stdout);
}


template<size_t n_elements>
class Collection;


class Parser {
 public:
	lxb_html_parser_t* const parser;
	
	Parser()
	: parser(lxb_html_parser_create())
	{
		assert_not_null(this->parser);
		if (lxb_html_parser_init(this->parser) != LXB_STATUS_OK)
			throw std::runtime_error("Cannot create parser");
	}
	
	~Parser(){
		lxb_html_parser_destroy(this->parser);
	}
};


class Doc {
  public:
	lxb_html_document_t* const document;
	
	Doc(const Parser& parser,  const char* html,  const size_t html_sz)
	: document(lxb_html_parse(parser.parser, reinterpret_cast<const lxb_char_t*>(html), html_sz))
	{
		assert_not_null(this->document);
	}
	
	~Doc(){
		lxb_html_document_destroy(this->document);
	}
	
	lxb_dom_document_t* get_dom_doc() const {
		return &this->document->dom_document;
	}
	
	template<size_t n_elements>
	bool get_elements_of_class_name(Collection<n_elements>& collection,  const char* name){
		printf("Looking for elements of class: %s\n", name);
		return (lxb_dom_elements_by_class_name(lxb_dom_interface_element(this->document), collection.collection, reinterpret_cast<const lxb_char_t*>(name), strlen(name)) != LXB_STATUS_OK);
	}
};


template<size_t n_elements>
class Collection {
  public:
	lxb_dom_collection_t* const collection;
	
	Collection(const Doc& doc)
	: collection(lxb_dom_collection_make(doc.get_dom_doc(), n_elements))
	{
		assert_not_null(this->collection);
	}
	
	~Collection(){
		lxb_dom_collection_destroy(this->collection, /*Suicide*/ true);
	}
	
	lxb_dom_element_t* at(const size_t i) const {
		return lxb_dom_collection_element(collection, i);
	}
	
	size_t length() const {
		return lxb_dom_collection_length(this->collection);
	}
};


class Element {
  public:
	lxb_dom_element_t* const element;
	
	Element(lxb_dom_element_t* _el)
	: element(_el)
	{
		assert_not_null(this->element);
	}
	
	template<size_t n>
	Element(const Collection<n>& collection,  size_t i)
	: Element(collection.at(i))
	{}
	
	void print_tag_name() const {
		size_t len = 0;
		const lxb_char_t* tag_name = lxb_dom_element_qualified_name(this->element, &len);
		printf("%.*s\n", (int)len, reinterpret_cast<const char*>(tag_name));
	}
	
	bool to_str(char* buf,  const size_t buf_sz) const {
		lexbor_str_t buf_view;
		buf_view.data = reinterpret_cast<lxb_char_t*>(buf);
		buf_view.length = sizeof(buf);
		return (lxb_html_serialize_str(lxb_dom_interface_node(this->element), &buf_view) != LXB_STATUS_OK);
	}
	
	void print_inner_text() const {
		log("value()");
		size_t len = 0;
		const lxb_char_t* tag_name = lxb_dom_element_qualified_name(lxb_dom_interface_element(this->element->node.first_child->next->first_child->next), &len);
		printf("%.*s\n", (int)len, reinterpret_cast<const char*>(tag_name));
	}
};


int main(int argc,  const char* const* argv){
	if (argc != 3){
		printf("Usage: [HTML_FILE] [CLASS_NAME]\n");
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
	
	el.print_inner_text();
	
	return 0;
}
