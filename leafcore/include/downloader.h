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
	bool					init();

	/**
	 * @brief	Downloads from the supplied url to the supplied stream
	 * @param	url			The URL to download from
	 * @param	out			The stream to supply the data to
	 * @return	Success
	 */
	bool					download(std::string url, std::ostream& out);

	/**
	 * @brief	Returns the last error of the downloader
	 */
	std::string				getError();

private:
	//A curl object to operate with
	CURL*					_curl;

	//The last error in clear text
	std::string				_error;
};

#endif