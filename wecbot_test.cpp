#include <libxml/HTMLparser.h>
#include <libxml/tree.h>

#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>

#include <iostream>
#include <string>
#include <cstdlib> // needed for ato[i/f] functions :( 
#include <vector>
#include <typeinfo>
#include <sstream>

class StatUser { 
	public: 
		int pos; 
		int number; 
		float pct; 
}; 

// TO compile:
// g++ -o wecbot_test wecbot_test.cpp -I /usr/include/libxml2/ -lxml2

void print_xpath_nodes_cpp(xmlNodeSetPtr nodes, std::ostream& output);
void print_xpath_nodes_test(xmlNodeSetPtr nodes); 
//void print_xpath_nodes(xmlNodeSetPtr nodes, FILE* output);

bool check4DirectRule(std::vector<StatUser>& statsUser, std::string& reason);
	
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
//	print_xpath_nodes(xpathObj->nodesetval, stdout); // Old C way


	print_xpath_nodes_test(xpathObj->nodesetval);

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
			} else {
				output << "= namespace \"" << ns->prefix << "\"=\"" << ns->href << "\" for node " << cur->name << std::endl;
			}
		} else if(nodes->nodeTab[i]->type == XML_ELEMENT_NODE) {
			cur = nodes->nodeTab[i];
			if(cur->ns) {
				output << "= element node \"" << cur->ns->href << ":" << cur->name << "\"" << std::endl; 
			} else {
				output << "= element node \"" << cur->name <<"\"" << std::endl; 
			}
		} else {
			cur = nodes->nodeTab[i];
			output << "= node \""<< cur->name << "\": type " << cur->type << std::endl;
		}
	}
}


/*void
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
}*/

void findData(xmlDoc* xmlDoc, const char *expr) {




}




void print_xpath_nodes_test(xmlNodeSetPtr nodes) { 
	xmlNodePtr cur;
	int size;
	int i;

	std::vector<StatUser> seasonStats; 
	std::vector<StatUser> globalStats; 

	size = (nodes) ? nodes->nodeNr : 0;

	std::cout << "Result (" << size << " nodes):" << std::endl; 

	for(i = 0; i < size; ++i) {
		if (!nodes->nodeTab[i]) continue;

		if (nodes->nodeTab[i]->type == XML_ELEMENT_NODE) { 
			
			const char* xpathExpr= "h4";
			
			xmlXPathContextPtr xpathCtx = xmlXPathNewContext(nodes->nodeTab[i]->doc);
			if (!xpathCtx) {
				std::cerr << "Unable to create context" << std::endl;
				return; 
			}
			
			xmlXPathObjectPtr xpathObj = xmlXPathNodeEval(nodes->nodeTab[i], (const xmlChar *)xpathExpr, xpathCtx); 
			
			if (xpathObj) { 
				if (xpathObj->nodesetval && xpathObj->nodesetval->nodeNr > 0) { 
					std::cout << "H4 has " << xpathObj->nodesetval->nodeNr << " elements" << std::endl; 
						
					for (int j = 0; j < xpathObj->nodesetval->nodeNr; j++) { 
						xmlNodePtr currNode = xpathObj->nodesetval->nodeTab[j]; 
						
						if (currNode->children && currNode->children->type == XML_TEXT_NODE) { 
							xmlChar * content = currNode->children->content; 
							std::string ctnt = std::string((char * ) content); 
							
							if (ctnt == "Rank-O-Meter Running Season") { 
								xmlXPathObjectPtr xpathTr = xmlXPathNodeEval(nodes->nodeTab[i], (const xmlChar *) "table/tr", xpathCtx); 
								
								if (xpathTr && xpathTr->nodesetval) { 
									std::cout << "TR has " << xpathTr->nodesetval->nodeNr << " elements" << std::endl; 

									for (int k = 1; k < xpathTr->nodesetval->nodeNr; k++) {  // element 0 is header
										xmlXPathObjectPtr xpathTd = xmlXPathNodeEval(xpathTr->nodesetval->nodeTab[k], (const xmlChar *) "td", xpathCtx); 
										std::cout << "TD has " << xpathTd->nodesetval->nodeNr << " elements" << std::endl; 

										StatUser stat; 
										bool parseable = true; 
										for (int l = 0; l < xpathTd->nodesetval->nodeNr; l++) { 
											
											char* data;
											try {
												data = (char*) xpathTd->nodesetval->nodeTab[l]->children->content;
											}
											catch (std::exception &cException) {std::cerr << "Standard exception at " << __LINE__ << ": " << cException.what() << "; " << typeid(cException).name() << std::endl;}
											
											switch (l) { 
												case 0: 
													stat.pos = std::atoi((char *)data);
													break; 
												case 1: 
													try { 
														stat.number = std::atoi((char *)data);
													}
													catch (std::invalid_argument &eInv) {parseable = false; stat.number = 0;} 
													break; 
												case 2:
													if (parseable)
														stat.pct = std::atof((char *)data);
													else 
														stat.pct = 0.0f; 
													break; 
												default: 
													break; 
											}
//											std::cout << "[k,l] = [" << k << "," << l << "]" << " ==> " << xpathTd->nodesetval->nodeTab[l]->children->content << std::endl; 
										}
										seasonStats.push_back(stat);
										xmlXPathFreeObject(xpathTd);
									}
								}

								xmlXPathFreeObject(xpathTr);
								std::cout << "Found \"Rank-O-Meter Running Season\" (i, j) = ("  << i << ", " << j << ")" << std::endl;
							}
							else if (ctnt == "All Time-Rank-O-Meter") { 
								xmlXPathObjectPtr xpathTr = xmlXPathNodeEval(nodes->nodeTab[i], (const xmlChar *) "table/tr", xpathCtx); 
								
								if (xpathTr && xpathTr->nodesetval) { 
									std::cout << "TR has " << xpathTr->nodesetval->nodeNr << " elements" << std::endl; 

									for (int k = 1; k < xpathTr->nodesetval->nodeNr; k++) {  // element 0 is header
										xmlXPathObjectPtr xpathTd = xmlXPathNodeEval(xpathTr->nodesetval->nodeTab[k], (const xmlChar *) "td", xpathCtx); 
										std::cout << "TD has " << xpathTd->nodesetval->nodeNr << " elements" << std::endl; 

										StatUser stat; 
										bool parseable = true; 
										for (int l = 0; l < xpathTd->nodesetval->nodeNr; l++) { 
											
											xmlChar* data;
											try {
												data = xpathTd->nodesetval->nodeTab[l]->children->content;
											}
											catch (std::exception &cException) {std::cerr << "Standard exception at " << __LINE__ << ": " << cException.what() << "; " << typeid(cException).name() << std::endl;}
											
											switch (l) { 
												case 0: 
													stat.pos = std::atoi((char *)data);
													break; 
												case 1: 
													try { 
														stat.number = std::atoi((char *)data);
													}
													catch (std::invalid_argument &eInv) {parseable = false; stat.number = 0;} 
													break; 
												case 2:
													if (parseable)
														stat.pct = std::atof((char *)data);
													else 
														stat.pct = 0.0f; 
													break; 
												default: 
													break; 
											}
//											std::cout << "[k,l] = [" << k << "," << l << "]" << " ==> " << xpathTd->nodesetval->nodeTab[l]->children->content << std::endl; 
										}
										globalStats.push_back(stat);
										xmlXPathFreeObject(xpathTd);
									}
								}

								xmlXPathFreeObject(xpathTr);
								std::cout << "Found \"All Time-Rank-O-Meter\" (i, j) = ("  << i << ", " << j << ")" << std::endl;
							}
						}
					}
				}
				else 
					std::cout << "H4 has no nodes" << std::endl; 
			}
			else 
				std::cout << "H4 is null" << std::endl; 
			
			xmlXPathFreeObject(xpathObj);
			xmlXPathFreeContext(xpathCtx);
			
		}
	}
	
	std::cout << "	Season Stats " << std::endl; 
	for (std::vector<StatUser>::iterator it = seasonStats.begin(); it != seasonStats.end(); ++it) { 
		std::cout << "Pos: " << it->pos << "; Count: " << it->number << "; Pct: " << it->pct << std::endl;
	} 

	std::cout << "	Global Stats " << std::endl; 
	for (std::vector<StatUser>::iterator it = globalStats.begin(); it != globalStats.end(); ++it) { 
		std::cout << "Pos: " << it->pos << "; Count: " << it->number << "; Pct: " << it->pct << std::endl;
	}
	
	std::string reason; 
	check4DirectRule(globalStats, reason); 
	
	std::cout << reason << std::endl;
	
}



bool check4DirectRule(std::vector<StatUser>& statsUser, std::string& reason) { 
	
	unsigned totGames = 0; 
	unsigned valGames = 0; 
	
	std::ostringstream buff; 
	bool result = false; 
	
	std::vector<StatUser>::iterator it_beg = statsUser.begin();
	std::vector<StatUser>::iterator it_end = statsUser.end();
	
	while (it_beg != it_end) { 
		totGames += it_beg->number; 
		if (it_beg->pos >= 1 && it_beg->pos <= 4) valGames += it_beg->number; 
		
		++it_beg;
	}

	// TODO: Need 2 seasons 

	if (totGames < 300) { 
		buff << "Not enough games " << totGames; 
	}
	else { 
		float pct = valGames/((double)totGames);
		
		if (pct >= 0.6) {
			buff << "Passes " << pct; 
			result = true; 
		}
		else { 
			buff << "NOT Passes " << pct; 
		}
	}

	reason = buff.str(); 
	return reason; 
}
