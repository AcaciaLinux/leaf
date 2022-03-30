/**
 * @file		downloader/download.cpp
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		The implementation of Downloader::download()
 * @copyright	Copyright (c) 2022
 */
#include "log.h"
#include "error.h"
#include "leafdebug.h"
#include "leafconfig.h"

#include "downloader.h"

static size_t writeFunc(void* ptr, size_t size, size_t nmemb, std::ostream *s){
	(*s).write(((char*)ptr), size*nmemb);

	return size*nmemb;
}

void Downloader::download(std::string url, std::ostream& out){
	FUN();

	LEAF_DEBUG_EX("Downloader::download()");

	CURLcode res;

	if (!_curl)
		throw new LeafError(Error::DL_NOT_INIT);

	if (!out.good())
		throw new LeafError(Error::DL_BAD_STREAM);

	curl_easy_setopt(_curl, CURLOPT_URL, url.c_str());
	curl_easy_setopt(_curl, CURLOPT_FOLLOWLOCATION, 1L);
	curl_easy_setopt(_curl, CURLOPT_WRITEFUNCTION, writeFunc);
	curl_easy_setopt(_curl, CURLOPT_WRITEDATA, &out);
	curl_easy_setopt(_curl, CURLOPT_NOPROGRESS, false);

	res = curl_easy_perform(_curl);

	if (res != CURLE_OK)
		throw new LeafError(Error::DL_CURL_ERR, curl_easy_strerror(res));
}
