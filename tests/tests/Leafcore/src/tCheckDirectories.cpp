#include "../t_leafcore.h"

TEST(Leafcore, checkDirectories_debug){
	FUN();

	LEAF_DEBUG_SET_FAIL("Leafcore::checkDirectories()", true);
	
	Leafcore core;
	
	if (core.checkDirectories())
		FAIL() << "Debug fail did not fail";
}