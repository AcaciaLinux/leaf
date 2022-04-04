#include "../t_pkglistparser.h"

#include <fstream>

TEST(PackageListParser, PackageListParser_debug){
	FUN();

	LEAF_DEBUG_SET_FAIL("PackageListParser::PackageListParser()");

	try{
		
		PackageListParser parser;

		F_NOTHROW(Error::DEBUG_EXCEPTION);
	} catch (LeafError* e){
		CHECK_EC(Error::DEBUG_EXCEPTION, e);
	} catch (...){
		F_WRONGEXCEPTION("LeafError*");
	}
}
