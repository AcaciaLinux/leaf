/**
 * @file		package/pre_postinstall.cpp
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		The code for running the preinstall and postinstall scripts
 * @copyright 	Copyright (c) 2022
 */

#include "log.h"
#include "fail.h"
#include "package.h"
#include "leafconfig.h"

#include <filesystem>

bool Package::runPreinstall(){
	return runScript("preinstall.sh");
}

bool Package::runPostinstall(){
	return runScript("postinstall.sh");	
}

bool Package::runScript(std::string path){
	FUN();
	_error.clear();
	std::string _ep = "Failed to run script for package " + getFullName() + ": ";

	//Check if the database is ok
	if (_db == nullptr){
		_error = _ep + "Database is not accessible (nullptr)";
		return FAIL(_error);
	}

	if (!_db->getCore()->createCacheDirs()){
		_error = _ep + _db->getCore()->getError();
		return FAIL(_error);
	}

	if (!std::filesystem::exists(getExtractedDir())){
		_error = _ep + "Package " + getFullName() + " does not seem to be extracted to " + getExtractedDir();
		return FAIL(_error);
	}

	if (!std::filesystem::exists(getExtractedDir() + path)){
		LOGI("Script " + getExtractedDir() + path + " does not exist");
		return true;
	}

	std::string oldWorkDir = std::filesystem::current_path();

	std::filesystem::current_path(getExtractedDir());

	int res = 0;

	std::string command = "bash -c \"export ROOTDIR=" + lConfig.rootDir + " && bash ./" + path + "\"";

	LOGI("Running command: \"" + command + "\" in " + getExtractedDir());

	if (hlog->getLevel() < Log::I)
		res = system((command + " >> /dev/null").c_str());
	else
		res = system(command.c_str());

	if (res != 0){
		_error = _ep + "Script failed to run";
		std::filesystem::current_path(oldWorkDir);
		return FAIL(_error);
	}

	std::filesystem::current_path(oldWorkDir);

	return true;
}
