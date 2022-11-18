#include "../t_parser.h"

#include <fstream>

TEST(Parser, get_debug){
	FUN();

	LEAF_DEBUG_SET_FAIL("Parser::get()");

	try{

		Parser parser;

		parser.get("", "");
	
		F_NOTHROW(Error::DEBUG_EXCEPTION);
	} catch (LeafError* e){
		CHECK_EC(Error::DEBUG_EXCEPTION, e);
	} catch (...){
		F_WRONGEXCEPTION("LeafError*");
	}
}

TEST(Parser, get_general){
	FUN();

	std::stringstream sstr = std::stringstream("myConfig=myConf");

	Parser parser;
	parser.parse(sstr);

	ASSERT_EQ("myConf", parser.get("myConfig", "myConf"));
	ASSERT_EQ("NoValue", parser.get("my", "NoValue"));

}
