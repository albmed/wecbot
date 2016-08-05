#include <libxml/tree.h>
#include <libxml/HTMLparser.h>
#include <libxml++/libxml++.h>

#include <sys/select.h>
#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
 
#include <iostream>
#include <string>
 
#define HEADER_ACCEPT "Accept:text/html,application/xhtml+xml,application/xml"
#define HEADER_USER_AGENT "User-Agent:Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.17 (KHTML, like Gecko) Chrome/24.0.1312.70 Safari/537.17"

class StatUser { 
	public: 
		int pos; 
		int number; 
		float pct; 
}; 

unsigned const SEASON = 0; 
unsigned const GLOBAL = 1; 

/* TODO: Transformant a map i movent a una funció */ 

typedef std::map<unsigned, StatUser> StatsUser; 

void printEx(std::exception &cException, int sourceLine) { 
	std::cerr << "Standard exception at " << sourceLine << ": " << cException.what() << "; " << typeid(cException).name() << std::endl;
}
 
int main() {
	
	try { 
		std::string url = "http://www.poker-heroes.com/profile.html?user=tahr";
		curlpp::Easy request;
 
		// Specify the URL
		request.setOpt(curlpp::options::Url(url));
 
		// Specify some headers
		std::list<std::string> headers;
		headers.push_back(HEADER_ACCEPT);
		headers.push_back(HEADER_USER_AGENT);
		request.setOpt(new curlpp::options::HttpHeader(headers));
		request.setOpt(new curlpp::options::FollowLocation(true));
		request.setOpt(new curlpp::options::SslVerifyPeer(false));
 
		// Configure curlpp to use stream
		std::ostringstream responseStream;
		curlpp::options::WriteStream streamWriter(&responseStream);
		request.setOpt(streamWriter);
 
		// Collect response
		
		request.perform();
		std::string re = responseStream.str();
 
		// Parse HTML and create a DOM tree
		xmlDoc* doc = htmlReadDoc((xmlChar*)re.c_str(), NULL, NULL, HTML_PARSE_RECOVER | HTML_PARSE_NOERROR | HTML_PARSE_NOWARNING);
 
		// Encapsulate raw libxml document in a libxml++ wrapper
		xmlNode* r = xmlDocGetRootElement(doc);
		xmlpp::Element* root = new xmlpp::Element(r);
 
		// Grab the IP address
		std::string xpath = "//*[@class=\"pf_container\"]";
		auto elements = root->find(xpath);
		
		
		StatsUser m_statsUser; 
		
		std::vector<StatUser> seasonStats; 
		std::vector<StatUser> globalStats; 
		
		std::cout << "In elements: " << elements.size() << std::endl;
		for (auto node : elements) { 
			auto elem = node->find("h4"); 
			std::string ctnt; 
			try {
				ctnt = ((xmlpp::TextNode*)(((xmlpp::ContentNode*)(elem[0]))->get_first_child()))->get_content();
				if (ctnt == "Rank-O-Meter Running Season") { 
					std::cout << "In current season" << std::endl; 
						
						auto trs = node->find("table/tr"); 
						auto counter = 0; 
						for(auto tr : trs) {
							if (!counter++) continue; 
							auto tds = tr->find("td"); 
							auto place = 0;
							StatUser stat; 
							bool parseable = true; 
							for (auto td: tds) { 
								++place; 
								std::string data;
								try {
									data = ((xmlpp::TextNode*)(((xmlpp::ContentNode*)(td))->get_first_child()))->get_content();
								}
								catch (std::exception &cException) {printEx(cException, __LINE__);}
								switch (place) { 
									case 1: 
										stat.pos = std::stoi(data);
										break; 
									case 2: 
										try { 
											stat.number = std::stoi(data);
										}
										catch (std::invalid_argument &eInv) {parseable = false; stat.number = 0;} 
										break; 
									case 3:
										if (parseable)
											stat.pct = std::stof(data);
										else 
											stat.pct = 0.0f; 
										break; 
									default: 
										break; 
								}
							}
							seasonStats.push_back(stat);
						}
				}
				else if (ctnt == "All Time-Rank-O-Meter") { 
					std::cout << "All time seasons" << std::endl; 
				}
			}
			catch (std::exception &cException) {printEx(cException, __LINE__);}
/*			for (auto elem_i : elem) {
				try {
				std::cout << ((xmlpp::TextNode*)(((xmlpp::ContentNode*)(elem_i))->get_first_child()))->get_content() << std::endl;
				}
				catch (std::exception &cException) {}
			}*/
		}
		
		for (auto stat : seasonStats) { 
			std::cout << "Pos: " << stat.pos << "; Count: " << stat.number << "; Pct: " << stat.pct << std::endl;
		}
 
		delete root;
		xmlFreeDoc(doc);
	}
	catch (curlpp::LogicError & e)
	{
		std::cout << "Logic Error: "  << e.what() << std::endl;
		return 2;
	}

	catch (curlpp::RuntimeError & e)
	{
		std::cout << "Runtime Error: "  << e.what() << std::endl;
		return 3;
	}
	catch (std::exception &cException)
	{
		std::cerr << "Standard exception: " << cException.what() << std::endl;
		return 4;
	}		
	catch(...) {
		std::cerr << "Abnormal termination" << std::endl; 
		return 5;
	}

	return 0;
}

