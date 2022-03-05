#include "../t_leafcore.h"

TEST(Leafcore, askUserOK_debug){
	FUN();
	lConfig.noAsk = true;

	LEAF_DEBUG_SET_FAIL("Leafcore::askUserOK()");
	
	Leafcore core;
	
	if (core.askUserOK(""))
		FAIL() << "Debug fail did not fail";
}

TEST(Leafcore, askUserOK_noAsk){
	FUN();

	LEAF_DEBUG_SET_FAIL("Leafcore::askUserOK()-noAsk");

	Leafcore core;

	//First check without noAsk
	lConfig.noAsk = false;
	if (core.askUserOK("") == true)
		FAIL() << "askUserOK gave unwanted ok";

	//Now check with noAsk set
	lConfig.noAsk = true;
	if (core.askUserOK("") == false)
		FAIL() << "askUserOK ignored leafconfig.noAsk";
}