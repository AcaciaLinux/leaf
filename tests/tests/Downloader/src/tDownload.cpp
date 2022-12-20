#include "../t_downloader.h"

#include <fstream>

TEST(Downloader, download_debug){
	FUN();

	LEAF_DEBUG_SET_FAIL("Downloader::download()");

	try{

		Downloader dl("noURL", std::cout, true);
		dl.download();
		
		F_NOTHROW(Error::DEBUG_EXCEPTION);
	} catch (LeafError* e){
		CHECK_EC(Error::DEBUG_EXCEPTION, e);
	} catch (...){
		F_WRONGEXCEPTION("LeafError*");
	}
}

TEST(Downloader, download_notInit){
	FUN();

	Downloader dl("noURL", std::cout, true);

	try{
		dl.download();

		F_NOTHROW(Error::DL_NOT_INIT);
	} catch (LeafError* e){
		CHECK_EC(Error::DL_NOT_INIT, e);
	} catch (...){
		F_WRONGEXCEPTION("LeafError*");
	}
}

TEST(Downloader, download_bad_stream){
	FUN();

	std::ofstream outFile;
	outFile.setstate(std::ios::badbit);
	Downloader dl("noURL", outFile, true);

	try{
		dl.init();
		dl.download();

		F_NOTHROW(Error::DL_BAD_STREAM);
	} catch (LeafError* e){
		CHECK_EC(Error::DL_BAD_STREAM, e);
	} catch (...){
		F_WRONGEXCEPTION("LeafError*");
	}
}

TEST(Downloader, download_noURL){
	FUN();

	std::ofstream outFile;
	outFile.open("DownloaderTestFile.txt", std::ios::out);
	Downloader dl("", outFile, true);

	try{
		dl.init();
		dl.download();

		F_NOTHROW(Error::DL_CURL_ERR);
	} catch (LeafError* e){
		CHECK_EC(Error::DL_CURL_ERR, e);
	} catch (...){
		F_WRONGEXCEPTION("LeafError*");
	}
}
