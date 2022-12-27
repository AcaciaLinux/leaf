#define FRIEND_LEAFCORE
#include "../t_package.h"

#include "leaffs.h"

#include <filesystem>
#include <fstream>
#include <iterator>

TEST(Package, runScript_debug){
	FUN();

	LEAF_DEBUG_SET_FAIL("Package::runScript()");

	try{
		Leafcore core;

		Package* newPackage = new Package("testpkg", "1");
		core._packageListDB->addPackage(newPackage);
		newPackage->runScript("test");

		F_NOTHROW(Error::DEBUG_EXCEPTION);
	} catch (LeafError* e){
		CHECK_EC(Error::DEBUG_EXCEPTION, e);
	} catch (...){
		F_WRONGEXCEPTION("LeafError*");
	}
}

TEST(Package, runScript_noDB){
	try{
		Package* newPackage = new Package("testpkg", "1");
		newPackage->runScript("test");

		F_NOTHROW(Error::NODB);
	} catch (LeafError* e){
		CHECK_EC(Error::NODB, e);
	} catch (...){
		F_WRONGEXCEPTION("LeafError*");
	}
}

TEST(Package, runScript_notExtracted){

	//Create a dummy leafcore
	leaf_config_t c;
	c.rootDir = "./root";
	Leafcore core(c);
	Package* newPackage = new Package("testpkg", "1");
	core._installedDB->addPackage(newPackage);

	bool exists = LeafFS::exists(newPackage->getExtractedDir());

	if (exists){
		LOGU("[test] Removing extracted directory " + newPackage->getExtractedDir() + " for test");
		LeafFS::remove_all(newPackage->getExtractedDir());
	}

	try {
		newPackage->runScript("test");

		F_NOTHROW(Error::PKG_NOTEXTRACTED);
	} catch (LeafError* e){
		CHECK_EC(Error::PKG_NOTEXTRACTED, e);
	} catch (...){
		F_WRONGEXCEPTION("LeafError*");
	}
}

TEST(Package, runScript_scriptExists){

	//Create a dummy leafcore
	leaf_config_t c;
	c.rootDir = "./root";
	Leafcore core(c);
	Package* newPackage = new Package("testpkg", "1");
	core._installedDB->addPackage(newPackage);

	{//Create the installedDir
		bool exists = LeafFS::exists(newPackage->getExtractedDir());

		if (!exists){
			LOGU("[test] Creating extracted directory " + newPackage->getExtractedDir() + " for test");
			LeafFS::create_directories(newPackage->getExtractedDir());
		}
	}

	{//Remove the script file for test
		bool exists = LeafFS::exists(newPackage->getExtractedDir() + "test.sh");

		if (!exists){
			LOGU("[test] Removing script " + newPackage->getExtractedDir() + "test.sh for test");
			LeafFS::create_directories(newPackage->getExtractedDir());
		}
	}

	{//Create the dummy script file
		std::error_code ec;
		std::ofstream outFile(newPackage->getExtractedDir() + "test.sh");
		if (!outFile.is_open())
			throw new LeafError(Error::OPENFILEW, "[test] Dummy package script");
	}

	//The function should at least reach the fileCreated breakpoint, not return earlier
	try {
		LEAF_DEBUG_SET_FAIL("Leafcore::runScript::fileCreated");
		newPackage->runScript("test.sh");

		FAIL() << "Leafcore::runScript() did return before 'fileCreated' breakpoint (might have skipped due to non existing script)";
	} catch (LeafError* e){
		if (e->getErrorCode() != Error::DEBUG_EXCEPTION)
			FAIL() << "Leafcore::runScript() did return before 'fileCreated' breakpoint (might have skipped due to non existing script)";
	} catch (...){
		F_WRONGEXCEPTION("LeafError*");
	}
}

//Checks if the runfile is constructed correctly
TEST(Package, runScript_runfile){

	//Create a dummy leafcore
	leaf_config_t c;
	c.rootDir = "./root/";
	Leafcore core(c);
	Package* newPackage = new Package("testpkg", "1");
	core._installedDB->addPackage(newPackage);

	bool exists = LeafFS::exists(newPackage->getExtractedDir());

	if (!exists){
		LOGU("[test] Creating extracted directory " + newPackage->getExtractedDir() + " for test");
		LeafFS::create_directories(newPackage->getExtractedDir());
	}

	{//Create the dummy script file
		LOGU("[test] Creating dummy script file " + newPackage->getExtractedDir() + "test.sh");
		std::error_code ec;
		std::ofstream outFile(newPackage->getExtractedDir() + "test.sh");
		if (!outFile.is_open())
			throw new LeafError(Error::OPENFILEW, "[test] Dummy package script");
	}

	try {
		LEAF_DEBUG_SET_FAIL("Leafcore::runScript::fileCreated");
		newPackage->runScript("test.sh");

		F_NOTHROW(Error::PKG_NOTEXTRACTED);
	} catch (LeafError* e){
		CHECK_EC(Error::DEBUG_EXCEPTION, e);

		std::ifstream inFile(newPackage->getExtractedDir() + "runscript-" + newPackage->getFullName() + ".sh");
		if (!inFile.is_open())
			throw new LeafError(Error::OPENFILER, "[test] Dummy package script");

		std::string str(std::istreambuf_iterator<char>{inFile}, {});

		std::string eDir = newPackage->getExtractedDir();
		std::string origStr = "#!/bin/sh\n";
		origStr += "export PKGROOT=" + eDir.replace(eDir.find(c.rootDir), c.rootDir.length(), "/") + "\n";
		origStr += newPackage->getExtractedDir() + "test.sh\n";
		origStr += "unset PKGROOT\n";

		if (str != origStr)
			FAIL() << "RunScript does not match expectation: \n" + origStr + "----- VS -----\n" + str;

	} catch (...){
		F_WRONGEXCEPTION("LeafError*");
	}
}
