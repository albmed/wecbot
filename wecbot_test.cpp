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

		StatUser() { 
			pos = 0; 
			number = 0; 
			pct = 0.0f; 
		}
}; 

class Season {
	public: 
		std::string seasonDate; 
		unsigned position; 
		float score; 
		unsigned playedGames; 
		unsigned wonGames; 
		float average; 
		
		Season() { 
			position = 0; 
			score = 0.0f; 
			playedGames = 0; 
			wonGames = 0; 
			average = 0.0f; 
		}
};

class DataUser { 
	public:
		std::vector<StatUser> currentSeasonStats; 
		std::vector<StatUser> globalStats; 
		std::vector<Season> previousSeasons; 
		Season currentSeasson; 
		unsigned totGames; 
		
		DataUser() { 
			totGames = 0;
		}
};


// TO compile:
// g++ -o wecbot_test wecbot_test.cpp -I /usr/include/libxml2/ -lxml2

void print_xpath_nodes_cpp(xmlNodeSetPtr nodes, std::ostream& output);
void print_xpath_nodes_test(xmlNodeSetPtr nodes); 
//void print_xpath_nodes(xmlNodeSetPtr nodes, FILE* output);

void getUserStats(std::vector<StatUser>& statsUser, xmlNodePtr node, xmlXPathContextPtr xpathCtx);
void getPreviousSeasonStats(std::vector<Season>& seasonData, xmlNodePtr node, xmlXPathContextPtr xpathCtx);
unsigned getTotalGames(std::vector<StatUser> stats) ;
bool check4DirectRule(DataUser& dataUser, std::string& reason);
bool check4VeteranRule(DataUser& dataUser, std::string& reason); 
	
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

	DataUser dataUser; 
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
								getUserStats(dataUser.currentSeasonStats, nodes->nodeTab[i], xpathCtx);
							}
							else if (ctnt == "All Time-Rank-O-Meter") { 
								getUserStats(dataUser.globalStats, nodes->nodeTab[i], xpathCtx); 
							}
						}
						else if (currNode->children && currNode->children->type == XML_ELEMENT_NODE && 
									currNode->children->children && currNode->children->children->type == XML_TEXT_NODE) { // Find for previous season
								std::string ctnt = std::string((char *) currNode->children->children->content); 
								
								if (ctnt.find("Previous Seasons Ranking Statistics") != std::string::npos) { 
									std::cout << "In HERE <------- " << std::endl; 
										getPreviousSeasonStats(dataUser.previousSeasons, nodes->nodeTab[i], xpathCtx);
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
	
	dataUser.totGames = getTotalGames(dataUser.globalStats); 
	std::cout << "=== Total Games: " << dataUser.totGames << " ===" << std::endl; 
	
	std::vector<StatUser> seasonStats = dataUser.currentSeasonStats; 
	std::cout << "=== Season Stats ===" << std::endl; 
	for (std::vector<StatUser>::iterator it = seasonStats.begin(); it != seasonStats.end(); ++it) { 
		std::cout << "Pos: " << it->pos << "; Count: " << it->number << "; Pct: " << it->pct << std::endl;
	} 

	std::vector<StatUser> globalStats = dataUser.globalStats; 
	std::cout << "=== Global Stats ===" << std::endl; 
	for (std::vector<StatUser>::iterator it = globalStats.begin(); it != globalStats.end(); ++it) { 
		std::cout << "Pos: " << it->pos << "; Count: " << it->number << "; Pct: " << it->pct << std::endl;
	}
	
	std::vector<Season> previousSeasons = dataUser.previousSeasons; 
	std::cout << "=== Previous Seasons ===" << std::endl; 
	for (std::vector<Season>::iterator it = previousSeasons.begin(); it != previousSeasons.end(); ++it) { 
		std::cout << "SeasonDate: " << it->seasonDate << "; position: " << it->position << "; score: " << it->score;
		std::cout << "; playedGames: " << it->playedGames << "; wonGames: " << it->wonGames << "; average: " << it->average << std::endl;
	}
	
	std::string reason; 
	check4DirectRule(dataUser, reason); 
	std::cout << reason << std::endl;
	
	check4VeteranRule(dataUser, reason);
	std::cout << reason << std::endl;
	
}

void getUserStats(std::vector<StatUser>& statsUser, xmlNodePtr node, xmlXPathContextPtr xpathCtx) { 

	xmlXPathObjectPtr xpathTr = xmlXPathNodeEval(node, (const xmlChar *) "table/tr", xpathCtx); 
	
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
			}
			statsUser.push_back(stat);
			xmlXPathFreeObject(xpathTd);
		}
	}

	xmlXPathFreeObject(xpathTr);
}

void getPreviousSeasonStats(std::vector<Season>& seasonData, xmlNodePtr node, xmlXPathContextPtr xpathCtx) { 
	
	xmlXPathObjectPtr xpathTr = xmlXPathNodeEval(node, (const xmlChar *) "table/tr", xpathCtx); 

	if (xpathTr && xpathTr->nodesetval) { 

		std::string currentSeason; 
		Season season; 	
		bool existsData = false; 
	
		for (int k = 0; k < xpathTr->nodesetval->nodeNr; k++) {  // 1 per line (not equivalent to season) 

			xmlXPathObjectPtr xpathTd = xmlXPathNodeEval(xpathTr->nodesetval->nodeTab[k], (const xmlChar *) "td", xpathCtx); 
			if (xpathTd->nodesetval->nodeNr < 2) continue; // Is a blank line. 
			
			std::string data; 
			std::string value; 
			
			try {
				data = std::string((char*) xpathTd->nodesetval->nodeTab[0]->children->content);
				value = std::string((char*) xpathTd->nodesetval->nodeTab[1]->children->content);
			}
			catch (std::exception &cException) {std::cerr << "Standard exception at " << __LINE__ << ": " << cException.what() << "; " << typeid(cException).name() << std::endl;}
			
			std::string seasonDate = data.substr(0, data.find(" ")); 
			
			{
				std::string beta = "Beta"; 
				std::string::size_type n = seasonDate.find(beta);
				if (n != std::string::npos) {  // Cas temporades "Beta" 
					seasonDate = data.substr(0, data.find(" ", beta.length() + 1)); 
				}
			}
			
			if (currentSeason != seasonDate) { 
				if (existsData) {
					seasonData.push_back(season);
					season = Season();  // call constructor for new object 
				}
				season.seasonDate = seasonDate; 
				currentSeason = seasonDate; 
				existsData = true; 
			}

			if (data.find("ranking position") != std::string::npos) { 
				season.position = std::atoi(value.c_str());  
			}
			else if (data.find("score") != std::string::npos) { 
				season.score = std::atof(value.c_str()); 
			}
			else if (data.find("played games") != std::string::npos) { 
				season.playedGames = std::atoi(value.c_str());
			}
			else if (data.find("won games") != std::string::npos) { 
				season.wonGames = std::atoi(value.c_str());
			}
			else if (data.find("average points") != std::string::npos) { 
				season.average = std::atof(value.c_str());
			}
			
			xmlXPathFreeObject(xpathTd);
		}
		seasonData.push_back(season);
	}	
	xmlXPathFreeObject(xpathTr);
	
}

unsigned getTotalGames(std::vector<StatUser> stats) { 
	
	unsigned count = 0; 

	std::vector<StatUser>::iterator it_beg = stats.begin();
	std::vector<StatUser>::iterator it_end = stats.end();
	
	while (it_beg != it_end) { 
		count += it_beg->number; 
		++it_beg;
	}
	
	return count; 
}


//bool check4DirectRule(std::vector<StatUser>& statsUser, std::string& reason) { 
bool check4DirectRule(DataUser& dataUser, std::string& reason) { 
	
	std::ostringstream buff; 
	bool result = false; 

	if (dataUser.previousSeasons.size() < 2) { 
		buff << "No enough Seasons (" << dataUser.previousSeasons.size() + 1 << ")"; 
	}
	else if (dataUser.totGames < 300) { 
		buff << "Not enough games (" << dataUser.totGames << ")"; 
	}
	else { 
		unsigned totGames = dataUser.totGames; 
		unsigned valGames = 0; 
	
		std::vector<StatUser>::iterator it_beg = dataUser.globalStats.begin();
		std::vector<StatUser>::iterator it_end = dataUser.globalStats.end();
		
		while (it_beg != it_end) { 
			if (it_beg->pos >= 1 && it_beg->pos <= 4) valGames += it_beg->number; 
			
			++it_beg;
		}
	
		if (totGames < 300) { 
			buff << "Not enough games (" << totGames << ")"; 
		}
		else { 
			float pct = static_cast<double>(valGames)/static_cast<double>(totGames);
			
			if (pct >= 0.6) {
				buff << "Passes " << pct; 
				result = true; 
			}
			else { 
				buff << "NOT Passes " << pct; 
			}
		}
	}

	reason = buff.str(); 
	return result; 
}


bool check4VeteranRule(DataUser& dataUser, std::string& reason) { 

	std::ostringstream buff; 
	bool result = false; 

	if (dataUser.previousSeasons.size() < 4) { 
		buff << "Not enough Seasons (" << dataUser.previousSeasons.size() << ")"; 
	}
	else if (dataUser.totGames < 1000) { 
		buff << "Not enough games (" << dataUser.totGames << ")"; 
	}
	else { 
		buff << "TODO :) "; 
	}

	reason = buff.str(); 
	return result; 
}