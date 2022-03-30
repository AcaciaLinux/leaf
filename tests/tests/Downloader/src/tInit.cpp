#include "../t_downloader.h"

TEST(Downloader, init_debug){
	FUN();

	LEAF_DEBUG_SET_FAIL("Downloader::init()");

	try{
		Downloader dl;
		dl.init();
		FAIL() << "Debugging exception was not thrown";
	} catch (LeafError* e){
		if (e->getErrorCode() != Error::DEBUG_EXCEPTION)
			FAIL() << "Debugging exception raised wrong exception";
	} catch (...){
		FAIL() << "Wrong exception was thrown";
	}
}


