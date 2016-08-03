#ifndef __PARSE_HTML_H__
#define __PARSE_HTML_H__

#include <string> 
#include <libxml/tree.h>
#include <libxml/HTMLparser.h>
#include <libxml++/libxml++.h>

class ParseHtml { 
public: 	
	ParseHtml(const std::string & ); 
	virtual ~ParseHtml(); 
	
	void Init(); 
	void Clear(); 
	
private: 
	std::string m_html;
	xmlDoc* doc; 
	xmlpp::Element* root; 
};

#endif
