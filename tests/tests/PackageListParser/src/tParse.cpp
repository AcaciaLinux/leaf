#include "../t_pkglistparser.h"

#include <fstream>

TEST(PackageListParser, parse_debug){
	FUN();

	LEAF_DEBUG_SET_FAIL("PackageListParser::parse()");

	try{
		
		PackageListParser parser;

		parser.parse(std::cin);

		F_NOTHROW(Error::DEBUG_EXCEPTION);
	} catch (LeafError* e){
		CHECK_EC(Error::DEBUG_EXCEPTION, e);
	} catch (...){
		F_WRONGEXCEPTION("LeafError*");
	}
}

TEST(PackageListParser, parse_bad_istream){
	FUN();

	std::ifstream inFile;
	inFile.setstate(std::ios::badbit);

	try{
		
		PackageListParser parser;
		parser.parse(inFile);

		F_NOTHROW(Error::PKGPRS_BAD_STREAM);
	} catch (LeafError* e){
		CHECK_EC(Error::PKGPRS_BAD_STREAM, e);
	} catch (...){
		F_WRONGEXCEPTION("LeafError*");
	}
}
