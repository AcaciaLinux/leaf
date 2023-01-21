/**
 * @file		package/pre_postinstall.cpp
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		The code for running the preinstall and postinstall scripts
 * @copyright 	Copyright (c) 2022
 */

#include "log.h"
#include "leafdebug.h"
#include "package.h"
#include "leafconfig.h"

#include "leaffs.h"

#include <filesystem>
#include <fstream>

void Package::runPreinstall(){
	FUN();
	LEAF_DEBUG_EX("Package::runPreinstall()");

	//Check if the database is ok
	if (_db == nullptr)
		throw new LeafError(Error::NODB);
	
	if (!_db->getCore()->getConfig().runPreinstall){
		LOGW("WARNING: Disabled preinstall script of package " + getFullName());
		return;
	}
	runScript("preinstall.sh");
}

void Package::runPostinstall(){
	FUN();
	LEAF_DEBUG_EX("Package::runPostinstall()");

	//Check if the database is ok
	if (_db == nullptr)
		throw new LeafError(Error::NODB);

	if (!_db->getCore()->getConfig().runPostinstall){
		LOGW("WARNING: Disabled postinstall script of package " + getFullName());
		return;
	}
	runScript("postinstall.sh");	
}

void Package::runScript(std::string path){
	FUN();
	LEAF_DEBUG_EX("Package::runScript()");

	//Check if the database is ok
	if (_db == nullptr)
		throw new LeafError(Error::NODB);

	_db->getCore()->createCacheDirs();

	if (!LeafFS::exists(getExtractedDir()))
		throw new LeafError(Error::PKG_NOTEXTRACTED);

	if (!LeafFS::exists(getExtractedDir() + path)){
		LOGI("Script " + getExtractedDir() + path + " does not exist, skipping");
		return;
	}

	//Cache the root directory
	std::string rootDir = this->_db->getCore()->getConfig().rootDir;

	//The extracted directory relative to the chroot environment
	std::string relExtractedDir = "/" + std::string(std::filesystem::relative(this->getExtractedDir(), rootDir)) + "/";

	{//Create the executed script
		std::ofstream outFile(getExtractedDir() + "runscript-" + getFullName() + ".sh");
		if (!outFile.is_open())
			throw new LeafError(Error::OPENFILEW, "RunScript file for package " + getFullName());

		//Construct the script
		outFile << "#!/bin/sh" << std::endl;
		outFile << "set -e" << std::endl;
		outFile << "export PKGROOT=" + relExtractedDir << std::endl;
		outFile << relExtractedDir + path << std::endl;
		outFile << "unset PKGROOT" << std::endl;
	}

	//A breakpoint for the tests to check the script
	LEAF_DEBUG_EX("Leafcore::runScript::fileCreated");

	std::string command = "bash " + relExtractedDir + "runscript-" + getFullName() + ".sh";

	int res = _db->getCore()->runCommand(command, getExtractedDir());

	//Check the return code
	if (res != 0)
		throw new LeafError(Error::PACKAGE_SCRIPT_FAILED, path);

}
