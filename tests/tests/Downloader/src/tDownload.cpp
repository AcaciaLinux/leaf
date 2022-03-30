#include "../t_downloader.h"

#include <fstream>

TEST(Downloader, download_debug){
	FUN();

	LEAF_DEBUG_SET_FAIL("Downloader::download()");

	try{

		Downloader dl;
		dl.download("noURL", std::cout);
		FAIL() << "Debugging exception was not thrown";

	} catch (LeafError* e){

		if (e->getErrorCode() != Error::DEBUG_EXCEPTION)
			FAIL() << "Debugging exception raised wrong exception";

	} catch (...){
		FAIL() << "Wrong exception was thrown";
	}
}

TEST(Downloader, download_notInit){
	FUN();

	Downloader dl;

	try{
		dl.download("noURL", std::cout);

		F_NOTHROW(Error::DL_NOT_INIT);
	} catch (LeafError* e){
		CHECK_EC(Error::DL_NOT_INIT, e);
	} catch (...){
		F_WRONGEXCEPTION("LeafError*");
	}
}

TEST(Downloader, download_bad_stream){
	FUN();

	Downloader dl;
	std::ofstream outFile;
	outFile.setstate(std::ios::badbit);

	try{
		dl.init();
		dl.download("noURL", outFile);

		F_NOTHROW(Error::DL_BAD_STREAM);
	} catch (LeafError* e){
		CHECK_EC(Error::DL_BAD_STREAM, e);
	} catch (...){
		F_WRONGEXCEPTION("LeafError*");
	}
}

TEST(Downloader, download_noURL){
	FUN();

	Downloader dl;
	std::ofstream outFile;
	outFile.open("DownloaderTestFile.txt", std::ios::out);

	try{
		dl.init();
		dl.download("noURL", outFile);

		F_NOTHROW(Error::DL_CURL_ERR);
	} catch (LeafError* e){
		CHECK_EC(Error::DL_CURL_ERR, e);
	} catch (...){
		F_WRONGEXCEPTION("LeafError*");
	}
}
