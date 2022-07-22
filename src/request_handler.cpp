#include <iostream>
#include <string>
#include <rapidjson/document.h>
#include "Struct/HtmlMemory.h"
#include <curl/curl.h>
#include <vector>

size_t write_to_mem(char* ptr, size_t size, size_t nmemb, void* userdata) {
	size_t real_size = size * nmemb;
	HtmlOnMemory* html = (HtmlOnMemory*)userdata;
	html->mem_size = real_size;
	for (int i = 0; i < real_size; i++) {
		html->html += ptr[i];
	}

	return real_size;
}

HtmlOnMemory get_html_doc(std::string url) {
	std::vector <CURLcode> error_code_list;
	CURL* handle = curl_easy_init();

	//Set CURL Option
	error_code_list.push_back(curl_easy_setopt(handle, CURLOPT_URL, url.c_str()));
	//Write the html or get result to memory
	error_code_list.push_back(curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_to_mem));
	HtmlOnMemory* html = new HtmlOnMemory;
	error_code_list.push_back(curl_easy_setopt(handle, CURLOPT_WRITEDATA, (void*)html));


	std::cout << "\n\n Check for Errors\n";
	for (CURLcode error_code : error_code_list) {
		std::cout << curl_easy_strerror(error_code) << std::endl;
	}

	//Perform the CURL
	CURLcode perform_code = curl_easy_perform(handle);
	std::cout << curl_easy_strerror(perform_code) << std::endl;

	//Clean Up
	curl_easy_cleanup(handle);

	return *html;

}