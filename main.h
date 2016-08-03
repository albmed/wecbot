#ifndef __MAIN_PTH_PARSER__
#define __MAIN_PTH_PARSER__

#ifdef __CYGWIN__
	#include <sys/select.h>
#endif
 
#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>

#include <iostream>
#include <string>
#include <typeinfo>
#include <sstream> 

void init (const char *); 
std::string getContents(const char *);

#endif 