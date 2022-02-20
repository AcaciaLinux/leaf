#include "../t_package.h"

#include <filesystem>

#include "leafconfig.h"

TEST(Package, noURL){
	lConfig.rootDir = "./root/";
	lConfig.noAsk = true;

	LeafDB db(new Leafcore);

	Package* newP = db.newPackage("Test", "1.0");
	
	if (newP->fetch())
		FAIL();
}
