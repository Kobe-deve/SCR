#ifndef HANDLER
#include "game_handler.h"
#endif

// define url based on mode 

#define ENDPOINT_URL "http://127.0.0.1:8080/"

// writing response to string 
size_t writeFunction(void* ptr, size_t size, size_t nmemb, std::string* data) {
	data->append((char*)ptr, size * nmemb);
	return size * nmemb;
}
		

void httpProvider::pingServer()
{
	curl = curl_easy_init();
			
	// set up connection handling 
	if(curl) 
	{
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 1L);
	
		// perform POST request 
		res = curl_easy_perform(curl);
    
		// check error
		if(res != CURLE_OK)
			connected = false;
		else 
			connected = true;
		curl_easy_cleanup(curl);
	}		
}
		
void httpProvider::testGET()
{
	if(curl)
	{
		curl = curl_easy_init();
			
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
				
				
		string response;
				
		// write response to response string 
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeFunction);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
       
		// perform GET request 
		res = curl_easy_perform(curl);
				
		// check for errors 
		if(res != CURLE_OK)
			fprintf(stderr, "\ncurl_easy_perform() failed: %s\n",curl_easy_strerror(res));
	
			curl_easy_cleanup(curl);
		}
}
		
httpProvider::httpProvider()
{
	url = ENDPOINT_URL;
	pingServer();
}
		