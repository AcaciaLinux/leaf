#ifndef __DOWNLOADER_H__
#define __DOWNLOADER_H__

#include <curl/curl.h>

#include <string>
#include <ostream>

class Downloader{

public:
	Downloader();
	~Downloader();

	/**
	 * @brief	Initializes curl and the downloader
	 * @return	Success
	 */
	void					init();

	/**
	 * @brief	Downloads from the supplied url to the supplied stream
	 * @param	url			The URL to download from
	 * @param	out			The stream to supply the data to
	 * @return	The HTTP response code
	 */
	size_t					download(std::string url, std::ostream& out);

#ifndef FRIEND_DOWNLOADER
private:
#endif

	//A curl object to operate with
	CURL*					_curl = nullptr;
};

#endif