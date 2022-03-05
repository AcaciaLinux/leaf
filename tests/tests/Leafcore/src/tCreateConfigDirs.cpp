#include "../t_leafcore.h"

TEST(Leafcore, createConfigDirs_debug){
	FUN();
	lConfig.noAsk = true;

	LEAF_DEBUG_SET_FAIL("Leafcore::createConfigDirs()");
	
	Leafcore core;
	
	if (core.createConfigDirs())
		FAIL() << "Debug fail did not fail";
}