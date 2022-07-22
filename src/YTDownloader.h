#pragma once

#ifndef YTDOWNLOADER
	#define YTDOWNLOADER
	
	//Standard Include
	#include <iostream>
	#include <string>

	//Dev Defined Include
	#include "Class/Video.h"
	#include "Struct/HtmlMemory.h"
	
	//instalation.cpp
	void install();

	//request.cpp
	HtmlOnMemory get_html_doc(std::string);

#endif


