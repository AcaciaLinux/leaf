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
#include "globals.h"
#include "progress.h"

size_t Downloader::writeFunc(void* ptr, size_t size, size_t nmemb, std::ostream *s){
	Downloader* dl = (Downloader*) s;

	dl->_outStream.write(((char*)ptr), size*nmemb);
	dl->_md5.update(((char*)ptr), size*nmemb);

	if (proceed)
		return size*nmemb;
	else
		return 0;
}

int Downloader::progressFunc(void* ptr, curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow){
	FUN();

	Downloader* self = (Downloader*)ptr;
	std::string preProgress;

	//If there is size information available, display it before the bar
	if (dltotal > 0){
		preProgress = LeafUtil::bytesToHuman(dlnow) + " / " + LeafUtil::bytesToHuman(dltotal);
	}

	//If we haven't any size information yet, set the size to 0 to display an empty bar
	if (dltotal == 0)
		dltotal = 1;

	Log::Progress* progress = (Log::Progress*)(self->_progress.get());
	progress->update(dltotal, dlnow, preProgress);

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
	curl_easy_setopt(_curl, CURLOPT_LOW_SPEED_LIMIT, 1000);
	curl_easy_setopt(_curl, CURLOPT_LOW_SPEED_TIME, 10);

	if (_noProgress){
		curl_easy_setopt(_curl, CURLOPT_NOPROGRESS, true);
	} else {
		curl_easy_setopt(_curl, CURLOPT_XFERINFOFUNCTION, progressFunc);
		curl_easy_setopt(_curl, CURLOPT_XFERINFODATA, this);
		curl_easy_setopt(_curl, CURLOPT_NOPROGRESS, false);

		Log::conf_Progress conf;
		conf.maxSteps = 50;
		conf.prefix = prefix;
		_progress = hlog->createProgress(conf);
	}

	curlRes = curl_easy_perform(_curl);
	_md5.finalize();
	LOGD("[Downloader][download] MD5 of fetched data: " + getMD5());

	if (curlRes != CURLE_OK)
		if (proceed)
			throw new LeafError(Error::DL_CURL_ERR, std::string(curl_easy_strerror(curlRes)));
		else
			throw new LeafError(Error::ABORT);

	size_t httpRes = 0;
	curl_easy_getinfo(_curl, CURLINFO_RESPONSE_CODE, &httpRes);

	LOGI("[Downloader][download] HTTP response code: " + std::to_string(httpRes));

	return httpRes;
}
