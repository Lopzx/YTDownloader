#include <iostream>
#include <string>
#include <rapidjson/document.h>
#include "Struct/HtmlMemory.h"
#include <curl/curl.h>
#include <vector>

//#include "Class/Audio.h"
#include "Class/Video.h"

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

Video get_video_from_html(std::string html) {
	std::string opening_needle = "ytInitialPlayerResponse = ";
	std::string closing_needle = "};";

	size_t opening_pos = html.find(opening_needle) + opening_needle.size();
	size_t closing_pos = html.find(closing_needle, opening_pos);
	std::string json = html.substr(opening_pos, closing_pos - opening_pos + closing_needle.size()-1);

	rapidjson::Document doc;
	doc.Parse(json.c_str());

	
	std::string title = doc["videoDetails"]["title"].GetString();
	std::string author = doc["videoDetails"]["author"].GetString();
	std::vector <AdaptiveStream> video_list;

	rapidjson::GenericArray arr = doc["streamingData"]["adaptiveFormats"].GetArray();
	int arr_size = arr.Size();

	//Video Link
	for (int i = 0; i < arr_size; i++) {
		AdaptiveStream link;
		std::string mime = arr[i]["mimeType"].GetString();

		link.quality = arr[i]["quality"].GetString();
		link.url = arr[i]["url"].GetString();

		link.mime_type = arr[i]["mimeType"].GetString();
		if (mime.find("mp4") != std::string::npos) {
			link.format = "mp4";
		}
		else if (mime.find("webm") != std::string::npos) {
			link.format = "webm";
		}

		(mime.find("audio") != std::string::npos) ? link.type = "audio" : link.type = "video";
		video_list.push_back(link);
	}
	return Video(title, author, video_list);
}