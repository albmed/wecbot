/**
 * section: Tree
 * synopsis: Navigates a tree to print element names
 * purpose: Parse a file to a tree, use xmlDocGetRootElement() to
 *          get the root element, then walk the document and print
 *          all the element name in document order.
 * usage: tree1 filename_or_URL
 * test: tree1 test2.xml > tree1.tmp && diff tree1.tmp $(srcdir)/tree1.res
 * author: Dodji Seketeli
 * copy: see Copyright for the status of this software.
 
 Compile: 
 	gcc -o xmlexample libxml2-example.c -I/usr/include/libxml2 -L/usr/lib -lxml2 -liconv -lm
 */
#include <stdio.h>
#include <assert.h>
#include <libxml/HTMLparser.h>
#include <libxml/tree.h>

#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>

#ifdef LIBXML_TREE_ENABLED

void print_xpath_nodes(xmlNodeSetPtr nodes, FILE* output); 
/*
 *To compile this file using gcc you can type
 *gcc `xml2-config --cflags --libs` -o xmlexample libxml2-example.c
 */

/**
 * print_element_names:
 * @a_node: the initial xml node to consider.
 *
 * Prints the names of the all the xml elements
 * that are siblings or children of a given xml node.
 */
static void
print_element_names(xmlNode * a_node)
{
	xmlNode *cur_node = NULL;

	for (cur_node = a_node; cur_node; cur_node = cur_node->next) {
		if (cur_node->type == XML_ELEMENT_NODE) {
			printf("node type: Element, name: %s\n", cur_node->name);
		}

		print_element_names(cur_node->children);
	}
}


/**
 * Simple example to parse a file called "file.xml", 
 * walk down the DOM, and print the name of the 
 * xml elements nodes.
 */
int
main(int argc, char **argv)
{
	xmlDoc *doc = NULL;
	xmlNode *root_element = NULL;

	if (argc != 2)
			return(1);

	/*
	 * this initialize the library and check potential ABI mismatches
	 * between the version it was compiled for and the actual shared
	 * library used.
	 */
	LIBXML_TEST_VERSION

	/*parse the file and get the DOM */
	doc = htmlReadFile(argv[1], NULL, HTML_PARSE_RECOVER | HTML_PARSE_NOERROR | HTML_PARSE_NOWARNING);

	if (doc == NULL) {
			printf("error: could not parse file %s\n", argv[1]);
	}

	xmlXPathContextPtr xpathCtx = xmlXPathNewContext(doc);
	
	if(xpathCtx == NULL) {
		fprintf(stderr,"Error: unable to create new XPath context\n");
		xmlFreeDoc(doc); 
		return(-1);
	}
	
	const char* xpathExpr= "//*[@class=\"pf_container\"]";
	xmlXPathObjectPtr xpathObj = xmlXPathEvalExpression(xpathExpr, xpathCtx);
	
	if(xpathObj == NULL) { 
		fprintf(stderr,"Error: unable to evaluate xpath expression \"%s\"\n", xpathExpr);
		xmlXPathFreeContext(xpathCtx); 
		xmlFreeDoc(doc); 
		return(-1);
	}
	
	print_xpath_nodes(xpathObj->nodesetval, stdout);

	/*free the document */
	xmlXPathFreeObject(xpathObj);
	xmlXPathFreeContext(xpathCtx); 
	xmlFreeDoc(doc);

	/*
	 *Free the global variables that may
	 *have been allocated by the parser.
	 */
	xmlCleanupParser();

	return 0;
}


int 
register_namespaces(xmlXPathContextPtr xpathCtx, const xmlChar* nsList) {
	xmlChar* nsListDup;
	xmlChar* prefix;
	xmlChar* href;
	xmlChar* next;
	
	assert(xpathCtx);
	assert(nsList);

	nsListDup = xmlStrdup(nsList);
	if(nsListDup == NULL) {
		fprintf(stderr, "Error: unable to strdup namespaces list\n");
		return(-1);	
	}
		
	next = nsListDup; 
	while(next != NULL) {
		/* skip spaces */
		while((*next) == ' ') next++;
		if((*next) == '\0') break;
	
		/* find prefix */
		prefix = next;
		next = (xmlChar*)xmlStrchr(next, '=');
		if(next == NULL) {
			fprintf(stderr,"Error: invalid namespaces list format\n");
			xmlFree(nsListDup);
			return(-1);	
		}
		*(next++) = '\0';	
		
		/* find href */
		href = next;
		next = (xmlChar*)xmlStrchr(next, ' ');
		if(next != NULL) {
			*(next++) = '\0';	
		}
	
		/* do register namespace */
		if(xmlXPathRegisterNs(xpathCtx, prefix, href) != 0) {
			fprintf(stderr,"Error: unable to register NS with prefix=\"%s\" and href=\"%s\"\n", prefix, href);
			xmlFree(nsListDup);
			return(-1);	
		}
	}
		
	xmlFree(nsListDup);
	return(0);
}
void
print_xpath_nodes(xmlNodeSetPtr nodes, FILE* output) {
	xmlNodePtr cur;
	int size;
	int i;
	
	assert(output);
	size = (nodes) ? nodes->nodeNr : 0;
	
	fprintf(output, "Result (%d nodes):\n", size);
	for(i = 0; i < size; ++i) {
		assert(nodes->nodeTab[i]);
		
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

#else
int main(void) {
		fprintf(stderr, "Tree support not compiled in\n");
		exit(1);
}
#endif
