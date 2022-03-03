#include "../t_package.h"

TEST(Package, Deploy_NoDB){
	Package package("", "");

	try {
		package.deploy();

		//If we get past deploy(), fail
		FAIL() << "Deploy did not fail without database";
	} catch (LeafError* e){
		if (e->getErrorCode() != Error::NODB)
			FAIL() << "Deploy without DB did not throw NODB error";
	}
}