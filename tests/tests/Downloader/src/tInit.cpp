#include "../t_downloader.h"

TEST(Downloader, init_debug){
	FUN();

	LEAF_DEBUG_SET_FAIL("Downloader::init()");

	try{
		Downloader dl;
		dl.init();
		
		F_NOTHROW(Error::DEBUG_EXCEPTION);
	} catch (LeafError* e){
		CHECK_EC(Error::DEBUG_EXCEPTION, e);
	} catch (...){
		F_WRONGEXCEPTION("LeafError*");
	}
}


