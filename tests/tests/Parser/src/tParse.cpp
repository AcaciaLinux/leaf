#include "../t_parser.h"

#include <fstream>

TEST(Parser, parse_debug){
	FUN();

	LEAF_DEBUG_SET_FAIL("Parser::parse()");

	try{

		Parser parser;

		parser.parse(std::cin);
		
		F_NOTHROW(Error::DEBUG_EXCEPTION);
	} catch (LeafError* e){
		CHECK_EC(Error::DEBUG_EXCEPTION, e);
	} catch (...){
		F_WRONGEXCEPTION("LeafError*");
	}
}
