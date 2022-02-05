#ifndef __DOWNLOADER_H__
#define __DOWNLOADER_H__

#include <curl/curl.h>

#include <string>
#include <ostream>

class Downloader{

public:
	Downloader();
	~Downloader();

	bool					init();

	/**
	 * @brief	Downloads from the supplied url to the supplied stream
	 * @param	url			The URL to download from
	 * @param	out			The stream to supply the data to
	 * @return	0	Success
	 * 			-1	not initialized
	 * 			-2	out stream not opened
	 * 			-3	curl error
	 */
	int						download(std::string url, std::ostream& out);

private:
	CURL*					_curl;

};

#endif