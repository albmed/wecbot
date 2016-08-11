#include <iostream> 
#include <fstream>
#include <string>
#include <vector>

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/split.hpp> 
#include <boost/algorithm/string/constants.hpp>

#include<boost/tokenizer.hpp>

void test_1(char *fileName); 
void test_2(char* fileName); 

int main(int argc, char** argv) { 
	
		if (argc < 2) {
		std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
		return 1;
	}

	test_1(argv[1]); 
	test_2(argv[1]);

}

void test_1(char *fileName) { 
	
	std::cout << "==== Using split ====" << std::endl; 
	
	std::ifstream file(fileName);
	std::string str; 
	
	while (std::getline(file, str)) {
		std::cout << "For " << str << " = "; 
		
		std::vector<std::string> splitted; 
		boost::split (splitted, str, boost::is_any_of("##"), boost::token_compress_on); 

		std::cout << "["; 
		for (std::vector<std::string>::iterator it = splitted.begin(); it != splitted.end(); ++it) { 
			std::cout << "\"" << (*it) << "\","; 
		}
		std::cout << "]; Size: " << splitted.size() << std::endl; 

	}
}

void test_2(char* fileName) {
	std::cout << "==== Using tokenizer ====" << std::endl; 
	
	std::ifstream file(fileName);
	std::string str; 
	
	while (std::getline(file, str)) {
		std::cout << "For " << str << " = "; 
		
		boost::char_separator<char> sep("##");
		boost::tokenizer<boost::char_separator<char> > tok(str, sep); 
		
		std::cout << "["; 
		for(boost::tokenizer<boost::char_separator<char> >::iterator it=tok.begin(); it!=tok.end();++it){
			std::cout << "\"" << (*it) << "\","; 
		}
		std::cout << "]; " << std::endl; 
	}
	
}
