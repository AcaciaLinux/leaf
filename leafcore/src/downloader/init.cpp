/**
 * @file		downloader/init.cpp
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		The implementation of Downlaoder::init()
 * @copyright	Copyright (c) 2022
 */
#include "log.h"
#include "error.h"
#include "leafdebug.h"
#include "leafconfig.h"

#include "downloader.h"

void Downloader::init(){
	FUN();
	_curl = curl_easy_init();
	
	if (!_curl){
		throw new LeafError(Error::DL_INIT);
	}

	LOGD("Initialized libcurl");
}
