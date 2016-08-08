#include <libxml/HTMLparser.h>
#include <libxml/tree.h>

#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>

#include <fstream> 
#include <iostream>
#include <stdio.h>

// TO compile:
// g++ -o wecbot_test wecbot_test.cpp -I /usr/include/libxml2/ -lxml2

void print_xpath_nodes_cpp(xmlNodeSetPtr nodes, std::ostream& output);
void print_xpath_nodes(xmlNodeSetPtr nodes, FILE* output);

int main(int argc, char** argv) {

	xmlDoc *doc = NULL;
	xmlNode *root_element = NULL;

	if (argc < 2) {
		std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
		return 1;
	}

	doc = htmlReadFile(argv[1], NULL, HTML_PARSE_RECOVER | HTML_PARSE_NOERROR | HTML_PARSE_NOWARNING);

	if (doc == NULL) {
		std::cerr << "error: could not parse file" << argv[1] << std::endl;
		return 1;
	}

	xmlXPathContextPtr xpathCtx = xmlXPathNewContext(doc);

	if(xpathCtx == NULL) {
		std::cerr << "Error: unable to create new XPath context" << std::endl;
		xmlFreeDoc(doc);
		return 1;
	}

	const char* xpathExpr= "//*[@class=\"pf_container\"]";
	xmlXPathObjectPtr xpathObj = xmlXPathEvalExpression((const xmlChar *)xpathExpr, xpathCtx);

	if(xpathObj == NULL) {
		std::cerr << "Error: unable to evaluate xpath expression" << xpathExpr << std::endl;
		xmlXPathFreeContext(xpathCtx);
		xmlFreeDoc(doc);
		return 1;
	}


	print_xpath_nodes_cpp(xpathObj->nodesetval, std::cout); // here can we set an ofstream 

	/*free the document */
	xmlXPathFreeObject(xpathObj);
	xmlXPathFreeContext(xpathCtx);
	xmlFreeDoc(doc);














	return 0;
}


void print_xpath_nodes_cpp(xmlNodeSetPtr nodes, std::ostream& output) { 
	xmlNodePtr cur;
	int size;
	int i;

	size = (nodes) ? nodes->nodeNr : 0;

	output << "Result (" << size << " nodes):" << std::endl; 

	for(i = 0; i < size; ++i) {
		if (!nodes->nodeTab[i]) continue;

		if(nodes->nodeTab[i]->type == XML_NAMESPACE_DECL) {
			xmlNsPtr ns;

			ns = (xmlNsPtr)nodes->nodeTab[i];
			cur = (xmlNodePtr)ns->next;
			if(cur->ns) {
				output << "= namespace\"" << ns->prefix << "\"=\"" << 
					ns->href << "\" for node " << cur->ns->href << ":" << cur->name << std::endl; 
//				fprintf(output, "= namespace \"%s\"=\"%s\" for node %s:%s\n",
//				ns->prefix, ns->href, cur->ns->href, cur->name);
			} else {
				output << "= namespace \"" << ns->prefix << "\"=\"" << ns->href << "\" for node " << cur->name << std::endl;
				fprintf(output, "= namespace \"%s\"=\"%s\" for node %s\n",
				ns->prefix, ns->href, cur->name);
			}
		} else if(nodes->nodeTab[i]->type == XML_ELEMENT_NODE) {
			cur = nodes->nodeTab[i];
			if(cur->ns) {
				fprintf(output, "= element node \"%s:%s\"\n",
				cur->ns->href, cur->name);
			} else {
				fprintf(output, "= element node \"%s\"\n",
				cur->name);
			}
		} else {
			cur = nodes->nodeTab[i];
			fprintf(output, "= node \"%s\": type %d\n", cur->name, cur->type);
		}
	}
*/
}


void
print_xpath_nodes(xmlNodeSetPtr nodes, FILE* output) {
	xmlNodePtr cur;
	int size;
	int i;

	size = (nodes) ? nodes->nodeNr : 0;

	fprintf(output, "Result (%d nodes):\n", size);
	for(i = 0; i < size; ++i) {
		if (!nodes->nodeTab[i]) continue;

		if(nodes->nodeTab[i]->type == XML_NAMESPACE_DECL) {
			xmlNsPtr ns;

			ns = (xmlNsPtr)nodes->nodeTab[i];
			cur = (xmlNodePtr)ns->next;
			if(cur->ns) {
				fprintf(output, "= namespace \"%s\"=\"%s\" for node %s:%s\n",
				ns->prefix, ns->href, cur->ns->href, cur->name);
			} else {
				fprintf(output, "= namespace \"%s\"=\"%s\" for node %s\n",
				ns->prefix, ns->href, cur->name);
			}
		} else if(nodes->nodeTab[i]->type == XML_ELEMENT_NODE) {
			cur = nodes->nodeTab[i];
			if(cur->ns) {
				fprintf(output, "= element node \"%s:%s\"\n",
				cur->ns->href, cur->name);
			} else {
				fprintf(output, "= element node \"%s\"\n",
				cur->name);
			}
		} else {
			cur = nodes->nodeTab[i];
			fprintf(output, "= node \"%s\": type %d\n", cur->name, cur->type);
		}
	}
}

void findData(xmlDoc* xmlDoc, const char *expr) {




}

