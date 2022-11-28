/**
 * @file		downloader/downloader.cpp
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		The core implementation of Downloader
 * @copyright 	Copyright (c) 2022
 */

#include "log.h"
#include "downloader.h"
#include "error.h"

Downloader::Downloader(bool noProgress) : _noProgress(noProgress){
	FUN();
}

Downloader::~Downloader(){
	FUN();

	if (_curl){
		curl_easy_cleanup(_curl);
		LOGD("Cleaned up libcurl");
	}
}
