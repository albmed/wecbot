/**************************************************************************************************************/
/* Conjunt de proves per muntar un wecbot, que sigui capaç                                                    */
/* de descobrir jugadors que cumpleixin els requisits                                                         */
/* Per compilar-ho:                                                                                           */
/*  g++ -std=c++11 -Wall -g -o main main.cpp parsehtml.cpp -I/usr/include/libxml2 -L/usr/lib -lxml2 -liconv \ */
/*      -lm -I/usr/include/libxml++-2.6 -I/usr/lib/libxml++-2.6/include -I/usr/include/libxml2 \              */
/*      -I/usr/include/glibmm-2.4 -I/usr/lib/glibmm-2.4/include -I/usr/include/glib-2.0 \                     */
/*      -I/usr/lib/glib-2.0/include -I/usr/include/sigc++-2.0 -I/usr/lib/sigc++-2.0/include -lxml++-2.6 \     */
/*      -lxml2 -lglibmm-2.4 -lgobject-2.0 -lglib-2.0 -lintl -lsigc-2.0 -lcurl  -L /usr/local/lib/ -lcurlpp    */
/**************************************************************************************************************/

#include "main.h" 
#include "parsehtml.h"

void init(const char* name) { 

	// Get HTML contents 
	std::string re = getContents(name);
	// Get root 
	ParseHtml parser(re); 
	parser.Init(); 
	
	parser.Clear();	
}

std::string getContents(const char *name) { 
	const std::string base_url = "http://www.poker-heroes.com/profile.html"; 
	
	std::string url = base_url + std::string("?user=") + name;
	curlpp::Easy request;

	request.setOpt(curlpp::options::Url(url));

	std::ostringstream responseStream;
	curlpp::options::WriteStream streamWriter(&responseStream);
	request.setOpt(streamWriter);

	request.perform();
	std::string re = responseStream.str();
	
	return re;
}


int main(int argc, char** argv) {

	try { 
		if(argc < 2)
		{
			std::cerr << "Wrong number of arguments" << std::endl 
				<< "Usage: " << argv[0] << ": user_name" << std::endl;
			return 1;
		}
		
		init (argv[1]);  
	}
	catch (std::exception &cException)
	{
		std::cerr << "Standard exception: " << cException.what() << std::endl;
		std::cerr << "Standard exception: " << cException.what() << "; type: " << typeid(cException).name() << std::endl;
		return 1;
	}		
	catch(...) {
		std::cerr << "Abnormal termination" << std::endl; 
		return 2;
	}

	return 0; 
} 