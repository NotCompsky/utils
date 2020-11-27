#pragma once

#include "doc.hpp"
#include "notnull.hpp"


Doc::Doc(Parser& parser,  const char* html,  const size_t html_sz)
: document(parser.parse(html, html_sz))
{
	assert_not_null(this->document);
}

Doc::~Doc(){
	lxb_html_document_destroy(this->document);
}

lxb_dom_document_t* Doc::get_dom_doc() const {
	return &this->document->dom_document;
}

template<size_t n_elements>
bool Doc::get_elements_of_class_name(Collection<n_elements>& collection,  const char* name) const {
	printf("Looking for elements of class: %s\n", name);
	return Element(lxb_dom_interface_element(this->document)).get_elements_of_class_name(collection, name);
}
