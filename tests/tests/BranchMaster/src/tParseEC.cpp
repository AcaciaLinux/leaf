#include "../t_branchmaster.h"

#include <fstream>

TEST(BranchMaster, parseEC_debug){
	FUN();

	LEAF_DEBUG_SET_FAIL("BranchMaster::parseEC()");

	try{
		BranchMaster::parseEC("test");
		
		F_NOTHROW(Error::DEBUG_EXCEPTION);
	} catch (LeafError* e){
		CHECK_EC(Error::DEBUG_EXCEPTION, e);
	} catch (...){
		F_WRONGEXCEPTION("LeafError*");
	}
}

//Checks if the regex is ok
TEST(BranchMaster, parseEC_regex_substr_match){
	FUN();

	ASSERT_EQ(BranchMaster::E_REQUEST, BranchMaster::parseEC("sdfg E_REQUESTdadsfg "));
}

//Tests if the error codes get matched correctly
TEST(BranchMaster, parseEC_match){
	FUN();

	ASSERT_EQ(BranchMaster::E_REQUEST, BranchMaster::parseEC("E_REQUEST"));
	ASSERT_EQ(BranchMaster::E_GET, BranchMaster::parseEC("E_GET"));
	ASSERT_EQ(BranchMaster::E_PKGNAME, BranchMaster::parseEC("E_PKGNAME"));
	ASSERT_EQ(BranchMaster::E_VERSION, BranchMaster::parseEC("E_VERSION"));
	ASSERT_EQ(BranchMaster::E_GENERAL, BranchMaster::parseEC("E_GENERAL"));
	ASSERT_EQ(BranchMaster::E_PACKAGE, BranchMaster::parseEC("E_PACKAGE"));
}
