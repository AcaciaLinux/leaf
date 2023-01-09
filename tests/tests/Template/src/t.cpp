#include "../t_.h"

#include <fstream>

TEST(, _debug){
	FUN();

	LEAF_DEBUG_SET_FAIL("::()");

	try{
		
		F_NOTHROW(Error::DEBUG_EXCEPTION);
	} catch (LeafError* e){
		CHECK_EC(Error::DEBUG_EXCEPTION, e);
	} catch (...){
		F_WRONGEXCEPTION("LeafError*");
	}
}
