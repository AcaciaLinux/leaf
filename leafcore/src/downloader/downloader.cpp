#include "log.h"
#include "downloader.h"

Downloader::Downloader(){
	FUN();
}

Downloader::~Downloader(){
	FUN();

	if (_curl)
		curl_easy_cleanup(_curl);
}

bool Downloader::init(){
	FUN();
	_curl = curl_easy_init();
	
	if (!_curl){
		LOGE("Failed to initialize curl");
		return false;
	}

	return true;
}

static size_t writeFunc(void* ptr, size_t size, size_t nmemb, std::ostream *s){
	LOGD("Received chunk \"" + std::string((char*)ptr) + "\"");

	*s << (char*)ptr;

	return size*nmemb;
}

/*
0	Success
-1	not initialized
-2	out stream not opened
-3	curl error
*/
int Downloader::download(std::string url, std::ostream& out){
	FUN();
	CURLcode res;

	if (!_curl){
		LOGE("curl was not initialized");
		return -1;
	}

	if (!out.good()){
		LOGE("Out stream is not ok");
		return -2;
	}

	curl_easy_setopt(_curl, CURLOPT_URL, "https://raw.githubusercontent.com/AcaciaLinux/leaf_packages/main/packages.list");
	curl_easy_setopt(_curl, CURLOPT_FOLLOWLOCATION, 1L);
	curl_easy_setopt(_curl, CURLOPT_WRITEFUNCTION, writeFunc);
	curl_easy_setopt(_curl, CURLOPT_WRITEDATA, &out);

	res = curl_easy_perform(_curl);

	if (res != CURLE_OK){
		LOGE("curl error");
		return -3;
	}

	return 0;
}