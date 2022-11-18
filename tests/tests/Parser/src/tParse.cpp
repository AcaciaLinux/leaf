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

//Tests if the parser can parse a general stream
TEST(Parser, parse_general){
	FUN();

	std::stringstream sstr = std::stringstream("myConfig=myConf");

	Parser parser;
	parser.parse(sstr);

	ASSERT_EQ("myConf", parser.get("myConfig", ""));
}

//Tests if the parser can handle leading and trailing spaces
TEST(Parser, parse_remove_spaces){
	FUN();

	{
		std::stringstream sstr = std::stringstream("  myConfig =myConf");

		Parser parser;
		parser.parse(sstr);

		ASSERT_EQ("myConf", parser.get("myConfig", "")) << "Parser did not strip leading and trailing spaces in key";
	}

	{
		std::stringstream sstr = std::stringstream("myConfig=  myConf ");

		Parser parser;
		parser.parse(sstr);

		ASSERT_EQ("myConf", parser.get("myConfig", "")) << "Parser did not strip leading and trailing spaces in value";
	}

	{
		std::stringstream sstr = std::stringstream("  myConfig  =  myConf ");

		Parser parser;
		parser.parse(sstr);

		ASSERT_EQ("myConf", parser.get("myConfig", "")) << "Parser did not strip leading and trailing spaces in key an value";
	}
}

//Tests if the parser does not remove spaces within a option
TEST(Parser, parse_wrong_spaces){
	FUN();

	{
		std::stringstream sstr = std::stringstream("my Config=myConf");

		Parser parser;
		parser.parse(sstr);

		ASSERT_EQ("", parser.get("myConfig", "")) << "Parser did strip space within key";
	}

	{
		std::stringstream sstr = std::stringstream("myConfig=my Conf");

		Parser parser;
		parser.parse(sstr);

		ASSERT_EQ("my Conf", parser.get("myConfig", "")) << "Parser did strip space within value";
	}

}

//Tests the correct handling of comments
TEST(Parser, parse_comment){
	FUN();

	LEAF_DEBUG_SET_FAIL("Parser::comment");

	try{

		std::stringstream sstr = std::stringstream("#my Config=myConf");
		Parser parser;

		parser.parse(sstr);
		
		F_NOTHROW(Error::DEBUG_EXCEPTION) << " -> Expected ignoring of commented line";
	} catch (LeafError* e){
		CHECK_EC(Error::DEBUG_EXCEPTION, e);
	} catch (...){
		F_WRONGEXCEPTION("LeafError*");
	}
}

//Tests the correct handling of missing keys
TEST(Parser, parse_nokey){
	FUN();

	LEAF_DEBUG_SET_FAIL("Parser::nokey");

	try{

		std::stringstream sstr = std::stringstream("=myConf");
		Parser parser;

		parser.parse(sstr);
		
		F_NOTHROW(Error::DEBUG_EXCEPTION) << " -> Expected continue on no key";
	} catch (LeafError* e){
		CHECK_EC(Error::DEBUG_EXCEPTION, e);
	} catch (...){
		F_WRONGEXCEPTION("LeafError*");
	}
}

//Tests the correct handling of missing values
TEST(Parser, parse_novalue){
	FUN();

	LEAF_DEBUG_SET_FAIL("Parser::novalue");

	try{

		std::stringstream sstr = std::stringstream("myConfig=");
		Parser parser;

		parser.parse(sstr);
		
		F_NOTHROW(Error::DEBUG_EXCEPTION) << " -> Expected continue on no value";
	} catch (LeafError* e){
		CHECK_EC(Error::DEBUG_EXCEPTION, e);
	} catch (...){
		F_WRONGEXCEPTION("LeafError*");
	}
}
