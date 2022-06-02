#include "../t_configparser.h"

#include <fstream>

TEST(ConfigParser, parse_debug){
	FUN();

	LEAF_DEBUG_SET_FAIL("ConfigParser::parse()");

	try{

		ConfigParser parser;

		parser.parse(std::cin);
		
		F_NOTHROW(Error::DEBUG_EXCEPTION);
	} catch (LeafError* e){
		CHECK_EC(Error::DEBUG_EXCEPTION, e);
	} catch (...){
		F_WRONGEXCEPTION("LeafError*");
	}
}
