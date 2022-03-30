#include "../t_package.h"

#include <filesystem>

#include "leafconfig.h"

TEST(Package, noURL){
	lConfig.rootDir = "./root/";
	lConfig.noAsk = true;

	LeafDB db(new Leafcore);

	Package* newP = db.newPackage("Test", "1.0");

	try{
		newP->fetch();

		FAIL() << "Did not fail";
	} catch (LeafError* e){
		if (e->getErrorCode() != Error::DL_CURL_ERR){
			FAIL() << "Wrong error thrown";
		}

		LOGD("Exception OK: " + e->getErrorCodeMessage());
	}
}
