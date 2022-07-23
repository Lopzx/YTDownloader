#pragma once
#include "Audio.h"
#include <string>
#include <vector>
#include <rapidjson/document.h>
#include <iostream>
#include <curl/curl.h>
#include <filesystem>
#include <array>
#include <fstream>


struct AdaptiveStream {
	std::string type;
	std::string mime_type;
	std::string format;
	std::string quality;
	std::string url;
};

struct Formats {
	std::string download_path;
	std::string name;
	std::string type;
	std::string quality;
};


class Video {
	std::string title;
	std::string author;
	std::vector <AdaptiveStream> video_list;

	static size_t perform_download(char* ptr, size_t size, size_t nmemb, void* userdata) {
		size_t real_size = size * nmemb;
		
		Formats& format = *((Formats*)userdata);

		std::fstream file;
		file.open(format.download_path+ '/' + format.name + '-' + format.quality + '-' + format.type + "." + "mp4", std::ios::binary | std::ios::out | std::ios::app);
		for (int i = 0; i < real_size; i++) {
			file << ptr[i];
		}

		return real_size;
	}

	int get_url_index(std::string type, std::string quality) {
		int idx = 0;
		
		for (int i = 0; i < video_list.size(); i++) {
			AdaptiveStream& video_format = video_list[i];
			if (video_format.quality == quality && video_format.type == type && video_list[i].mime_type.find("mp4") != std::string::npos) {
				return i;
			}
		}

		return idx;
	}

public:

	Video(std::string title, std::string author, std::vector<AdaptiveStream> video_list) {
		this->title = title;
		this->author = author;
		this->video_list = video_list;
	}

	void get_video_format_list() {
		size_t vect_size = video_list.size();
		for (int i = 0; i < vect_size; i++) {
			if (video_list[i].type == "video" && video_list[i].mime_type.find("mp4") != std::string::npos) {
				std::cout << "Video Quality List : " << video_list[i].quality << std::endl;
			}
		}
	}

	void get_audio_format_list() {
		size_t vect_size = video_list.size();
		for (int i = 0; i < vect_size; i++) {
			if (video_list[i].type == "audio" && video_list[i].mime_type.find("mp4") != std::string::npos) {
				std::cout << "Audio Quality List : " << video_list[i].quality << " " << video_list[i].mime_type << std::endl;
			}
		}
	}

	void download(std::string type, std::string quality) {
		std::vector <CURLcode> error_code_list;
		CURL* handle = curl_easy_init();

		std::string path = "download/" + title;
		if (!std::filesystem::exists(path)) {
			std::filesystem::create_directories(path);
		}

		int url_index = get_url_index(type, quality);

		//Set CURL Option
		error_code_list.push_back(curl_easy_setopt(handle, CURLOPT_URL, video_list[url_index].url.c_str()));
		//Write the html or get result to disc
		error_code_list.push_back(curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, perform_download));
		Formats* format = new Formats();
		format->download_path = path;
		format->name = title;
		format->type = type;
		format->quality = quality;
		error_code_list.push_back(curl_easy_setopt(handle, CURLOPT_WRITEDATA, (void*)format));


		std::cout << "\n\n Check for Errors\n";
		for (CURLcode error_code : error_code_list) {
			std::cout << curl_easy_strerror(error_code) << std::endl;
		}

		//Perform the CURL
		CURLcode perform_code = curl_easy_perform(handle);
		std::cout << curl_easy_strerror(perform_code) << std::endl;

		//Clean Up
		curl_easy_cleanup(handle);
	}


	void print_details() {
		std::cout << "Title : " << title << std::endl;
		std::cout << "Author : " << author << std::endl;
		size_t vect_size = video_list.size();
		std::cout << "Video_List : \n";
		for (int i = 0; i < vect_size; i++) {
			std::cout << "Type : " << video_list[i].type << std::endl;
			std::cout << "Mime : " << video_list[i].mime_type << std::endl;
			std::cout << "Format : " << video_list[i].format << std::endl;
			std::cout << "Quality : " << video_list[i].quality << std::endl;
			std::cout << "Url : " << video_list[i].url << std::endl;
		}
	}
};