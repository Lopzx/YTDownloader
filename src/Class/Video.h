#pragma once
#include "Audio.h"
#include <string>
#include <vector>
#include <rapidjson/document.h>
#include <iostream>

struct AdaptiveStream {
	std::string type;
	std::string mime_type;
	std::string format;
	std::string quality;
	std::string url;
};


class Video {
	std::string title;
	std::string author;
	std::vector <AdaptiveStream> video_list;

public:

	Video(std::string title, std::string author, std::vector<AdaptiveStream> video_list) {
		this->title = title;
		this->author = author;
		this->video_list = video_list;
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