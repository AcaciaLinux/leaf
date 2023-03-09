#ifndef __DOWNLOADER_H__
#define __DOWNLOADER_H__

#include <curl/curl.h>

#include <string>
#include <ostream>
#include <memory>

#include "md5.h"

class Downloader{

public:
	/**
	 * @brief	Creates a new Downloader instance
	 * @param	url			The URL to the download file
	 * @param	outStream	The output stream
	 * @param	noProgress	If the downloader should not display any progress
	 */
	Downloader(std::string url, std::ostream& outStream, bool noProgress = false);

	/**
	 * @brief	Cleans up everything
	 */
	~Downloader();

	/**
	 * @brief	Initializes curl and the downloader
	 * @return	Success
	 */
	void					init();

	/**
	 * @brief	Downloads from the supplied url to the supplied stream
	 * @param	prefix		The prefix text to the progress bar
	 * @return	The HTTP response code
	 */
	size_t					download(std::string prefix = "");

	/**
	 * @brief	Returns the MD5 hash of the downloaded content
	 */
	std::string				getMD5() ;

#ifndef FRIEND_DOWNLOADER
private:
#endif

	/**
	 * @brief	The CURL object to work with
	 */
	CURL*					_curl = nullptr;

	/**
	 * @brief	If the animated progress bar should not be displayed
	 */
	bool					_noProgress = false;

	/**
	 * @brief	The stream the downloader outputs its downloaded content to
	 */
	std::ostream&			_outStream;

	/**
	 * @brief	The url to fetch the content from
	 */
	std::string				_url;

	/**
	 * @brief	The md5 hash of the fetched content
	 */
	MD5						_md5;

	/// @brief	The Log::Progress instance for this Downloader
	std::shared_ptr<void>	_progress;

	/**
	 * @brief	The function that gets called to write data
	 */
	static size_t			writeFunc(void* ptr, size_t size, size_t nmemb, std::ostream *s);

	/**
	 * @brief	The progress function libcurl calls for progress information
	 */
	static int				progressFunc(void* ptr, curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow);
};

#endif