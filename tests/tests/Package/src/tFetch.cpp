#include "../t_package.h"

#include <filesystem>

#include "leafconfig.h"

TEST(Package, noURL){
	LeafDB db(new Leafcore);

	db.getCore()->getConfig().rootDir = "./root/";
	db.getCore()->getConfig().noAsk = true;

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
