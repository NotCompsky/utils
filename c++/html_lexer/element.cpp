#pragma once

#include "element.hpp"
#include "collection_h.hpp"
#include "str_utils.hpp"
#include "notnull.hpp"


Element::Element(lxb_dom_element_t* _el)
: element(_el)
{
	assert_not_null(this->element);
}


bool Element::is_null() const {
	return this->element == nullptr;
}

bool Element::has_tag_name(const char* const tag_name) const {
	size_t len = 0;
	const char* const actual_tag_name = reinterpret_cast<const char*>(lxb_dom_element_qualified_name(this->element, &len));
	return streq(tag_name, actual_tag_name, len);
}

void Element::print_tag_name() const {
	size_t len = 0;
	const lxb_char_t* tag_name = lxb_dom_element_qualified_name(this->element, &len);
	printf("%.*s\n", (int)len, reinterpret_cast<const char*>(tag_name));
}

bool Element::to_str(char* buf,  size_t& buf_sz) const {
	lexbor_str_t buf_view;
	buf_view.data = reinterpret_cast<lxb_char_t*>(buf);
	buf_view.length = buf_sz;
	if (lxb_html_serialize_str(lxb_dom_interface_node(this->element), &buf_view) != LXB_STATUS_OK)
		return true;
	buf_sz = buf_view.length;
	return false;
}

void Element::print() const {
	char buf[1000];
	size_t buf_len = sizeof(buf)-1;
	this->to_str(buf, buf_len);
	printf("%.*s\n", (int)buf_len, buf);
}

lxb_dom_element_t* Element::get_element_given_tag_class_attr_indx(const char* tag_name,  const char* class_name,  const char* attr_name,  const char* attr_val,  const unsigned indx) const {
	// WARNING: Assumes at least one of tag/class are given
	Collection<1000> collection;
	if (class_name){
		this->get_elements_of_class_name(collection, class_name);
		if (tag_name){
			
		}
	} else if (tag_name)
		this->get_elements_of_tag_name(collection, tag_name);
	
}

lxb_dom_element_t* Element::get_direct_child_element_given_tag_class_attr_indx(const char* tag_name,  const char* class_name,  const char* attr_name,  const char* attr_val,  const unsigned indx) const {
	
}
