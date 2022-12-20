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

#include "util.h"

size_t Downloader::writeFunc(void* ptr, size_t size, size_t nmemb, std::ostream *s){
	Downloader* dl = (Downloader*) s;

	dl->_outStream.write(((char*)ptr), size*nmemb);
	dl->_md5.update(((char*)ptr), size*nmemb);

	return size*nmemb;
}

static int progressFunc(void* ptr, double dltotal, double dlnow, double ultotal, double ulnow){
	FUN();

	LeafUtil::Progress::print(*((std::string*)ptr), (uint64_t)dltotal, (uint64_t)dlnow, 50);

	return 0;
}

size_t Downloader::download(std::string prefix){
	FUN();

	LEAF_DEBUG_EX("Downloader::download()");

	LOGI("[Downloader][download] Downloading from \"" + _url + "\"...");

	CURLcode curlRes;

	if (!_curl)
		throw new LeafError(Error::DL_NOT_INIT);

	if (!_outStream.good())
		throw new LeafError(Error::DL_BAD_STREAM);

	curl_easy_setopt(_curl, CURLOPT_URL, _url.c_str());
	curl_easy_setopt(_curl, CURLOPT_FOLLOWLOCATION, 1L);
	curl_easy_setopt(_curl, CURLOPT_WRITEFUNCTION, writeFunc);
	curl_easy_setopt(_curl, CURLOPT_WRITEDATA, this);

	if (_noProgress){
		curl_easy_setopt(_curl, CURLOPT_NOPROGRESS, true);
	} else {
		curl_easy_setopt(_curl, CURLOPT_PROGRESSFUNCTION, progressFunc);
		curl_easy_setopt(_curl, CURLOPT_PROGRESSDATA, &prefix);
		curl_easy_setopt(_curl, CURLOPT_NOPROGRESS, false);
	}

	if (!_noProgress)
		LeafUtil::Progress::init();

	curlRes = curl_easy_perform(_curl);
	_md5.finalize();

	if (!_noProgress)
		LeafUtil::Progress::end();

	if (curlRes != CURLE_OK)
		throw new LeafError(Error::DL_CURL_ERR, curl_easy_strerror(curlRes));

	size_t httpRes = 0;
	curl_easy_getinfo(_curl, CURLINFO_RESPONSE_CODE, &httpRes);

	LOGI("[Downloader][download] HTTP response code: " + std::to_string(httpRes));

	return httpRes;
}
