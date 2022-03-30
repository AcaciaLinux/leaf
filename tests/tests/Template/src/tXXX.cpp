#include "../t_xxx.h"

#include <fstream>

TEST(XXX, xxx_debug){
	FUN();

	LEAF_DEBUG_SET_FAIL("XXX::xxx()");

	try{
		
		F_NOTHROW(Error::DEBUG_EXCEPTION);
	} catch (LeafError* e){
		CHECK_EC(Error::DEBUG_EXCEPTION, e);
	} catch (...){
		F_WRONGEXCEPTION("LeafError*");
	}
}
