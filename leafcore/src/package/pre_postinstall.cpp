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

#include <filesystem>

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

	if (!std::filesystem::exists(getExtractedDir()))
		throw new LeafError(Error::PKG_NOTEXTRACTED);

	if (!std::filesystem::exists(getExtractedDir() + path)){
		LOGI("Script " + getExtractedDir() + path + " does not exist, skipping");
		return;
	}

	std::string oldWorkDir = std::filesystem::current_path();

	std::filesystem::current_path(getExtractedDir());

	int res = 0;

	std::string envs = "";

	envs += " ROOTDIR=" + _db->getCore()->getConfig().rootDir;
	envs += " PKGROOT=" + this->getExtractedDir();

	std::string command = "bash -c \"" + envs + " ./" + path + "\"";

	LOGI("Running command: \"" + command + "\" in " + getExtractedDir());

	if (hlog->getLevel() < Log::I)
		res = system(command.c_str());
	else
		res = system(command.c_str());

	if (res != 0){
		std::filesystem::current_path(oldWorkDir);
		throw new LeafError(Error::PACKAGE_SCRIPT_FAILED, path);
	}

	std::filesystem::current_path(oldWorkDir);
}
