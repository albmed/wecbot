/**
 * example C code using libcurl and json-c
 * to post and return a payload using
 * http://jsonplaceholder.typicode.com
 *
 * Requirements:
 *
 * libcurl - http://curl.haxx.se/libcurl/c
 *
 * Build:
 *
 * g++ -g -o json_example json_example.cpp -lcurl
 *
 * Run:
 *
 * ./curltest
 * 
 */

/* standard includes */
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

/* libcurl (http://curl.haxx.se/libcurl/c) */
#include <curl/curl.h>

#include <iostream> 
#include <sstream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

/* holder for curl fetch */
struct curl_fetch_st {
	char *payload;
	size_t size;
};

/* callback for curl fetch */
size_t curl_callback (void *contents, size_t size, size_t nmemb, void *userp) {
	size_t realsize = size * nmemb;                             /* calculate buffer size */
	struct curl_fetch_st *p = (struct curl_fetch_st *) userp;   /* cast pointer to fetch struct */

	/* expand buffer */
	p->payload = (char *) realloc(p->payload, p->size + realsize + 1);

	/* check buffer */
	if (p->payload == NULL) {
		/* this isn't good */
		fprintf(stderr, "ERROR: Failed to expand buffer in curl_callback");
		/* free buffer */
		free(p->payload);
		/* return */
		return -1;
	}

	/* copy contents to buffer */
	memcpy(&(p->payload[p->size]), contents, realsize);

	/* set new buffer size */
	p->size += realsize;

	/* ensure null termination */
	p->payload[p->size] = 0;

	/* return size */
	return realsize;
}

/* fetch and return url body via curl */
CURLcode curl_fetch_url(CURL *ch, const char *url, struct curl_fetch_st *fetch) {
	CURLcode rcode;                   /* curl result code */

	/* init payload */
	fetch->payload = (char *) calloc(1, sizeof(fetch->payload));

	/* check payload */
	if (fetch->payload == NULL) {
		/* log error */
		fprintf(stderr, "ERROR: Failed to allocate payload in curl_fetch_url");
		/* return error */
		return CURLE_FAILED_INIT;
	}

	/* init size */
	fetch->size = 0;

	/* set url to fetch */
	curl_easy_setopt(ch, CURLOPT_URL, url);

	/* set calback function */
	curl_easy_setopt(ch, CURLOPT_WRITEFUNCTION, curl_callback);

	/* pass fetch struct pointer */
	curl_easy_setopt(ch, CURLOPT_WRITEDATA, (void *) fetch);

	/* set default user agent */
	curl_easy_setopt(ch, CURLOPT_USERAGENT, "libcurl-agent/1.0");

	/* set timeout */
	curl_easy_setopt(ch, CURLOPT_TIMEOUT, 5);

	/* enable location redirects */
	curl_easy_setopt(ch, CURLOPT_FOLLOWLOCATION, 1);

	/* set maximum allowed redirects */
	curl_easy_setopt(ch, CURLOPT_MAXREDIRS, 1);
	
	//curl_easy_setopt(ch, CURLOPT_NOPROXY, 1);

	/* fetch the url */
	rcode = curl_easy_perform(ch);

	/* return */
	return rcode;
}

void writeJson(std::string& json) { 
	boost::property_tree::ptree pt; 
	pt.put("username", "user"); 
	pt.put("password", "passwd"); 
	
	std::ostringstream buf; 
	boost::property_tree::write_json(buf, pt, true); 

	json = buf.str(); 
	
	std::cout << "JSON: " << json << std::endl; 
}


int main(int argc, char *argv[]) {
	CURL *ch;                                               /* curl handle */
	CURLcode rcode;                                         /* curl result code */

	struct curl_fetch_st curl_fetch;                        /* curl fetch struct */
	struct curl_fetch_st *cf = &curl_fetch;                 /* pointer to fetch struct */
	struct curl_slist *headers = NULL;                      /* http headers to send with request */

	/* url to test site */
	std::string url = "localhost/wec/sample.php";

	/* init curl handle */
	if ((ch = curl_easy_init()) == NULL) {
		/* log error */
		fprintf(stderr, "ERROR: Failed to create curl handle in fetch_session");
		/* return error */
		return 1;
	}

	/* set content type */
	headers = curl_slist_append(headers, "Accept: application/json");
	headers = curl_slist_append(headers, "Content-Type: application/json");

	std::string json; 
	writeJson(json);

	/* set curl options */
	curl_easy_setopt(ch, CURLOPT_CUSTOMREQUEST, "POST");
	curl_easy_setopt(ch, CURLOPT_HTTPHEADER, headers);
	curl_easy_setopt(ch, CURLOPT_POSTFIELDS, json.c_str());

	/* fetch page and capture return code */
	rcode = curl_fetch_url(ch, url.c_str(), cf);

	/* cleanup curl handle */
	curl_easy_cleanup(ch);

	/* free headers */
	curl_slist_free_all(headers);

	/* check return code */
	if (rcode != CURLE_OK || cf->size < 1) {
		/* log error */
		fprintf(stderr, "ERROR: Failed to fetch url (%s) - curl said: %s",
				url, curl_easy_strerror(rcode));
		/* return error */
		return 2;
	}

	/* check payload */
	if (cf->payload != NULL) {
		/* print result */
		printf("CURL Returned: \n%s\n", cf->payload);
		/* parse return */
		free(cf->payload);
	} else {
		/* error */
		fprintf(stderr, "ERROR: Failed to populate payload");
		/* free payload */
		free(cf->payload);
		/* return */
		return 3;
	}

	/* exit */
	return 0;
}
