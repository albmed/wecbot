#include "parsehtml.h" 

ParseHtml::ParseHtml(const std::string & html):m_html(html) { 
}

ParseHtml::~ParseHtml() { }


void ParseHtml::Init() { 
	// Parse HTML and create a DOM tree
	xmlDoc* doc = htmlReadDoc((xmlChar*)m_html.c_str(), NULL, NULL, HTML_PARSE_RECOVER | HTML_PARSE_NOERROR | HTML_PARSE_NOWARNING);

	// Encapsulate raw libxml document in a libxml++ wrapper
	xmlNode* r = xmlDocGetRootElement(doc);
	root = new xmlpp::Element(r);

	auto elements = root->find("//*[@class=\"pf_container\"]");
}

void ParseHtml::Clear() { 
	delete root; 
	xmlFreeDoc(doc);
}

