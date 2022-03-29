/**
 * @file		package/extract.cpp
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		The code for extracting a downloaded package for further processing
 * @copyright 	Copyright (c) 2022
 */

#include "log.h"
#include "fail.h"
#include "package.h"
#include "leafarchive.h"
#include "leafconfig.h"

#include <filesystem>

//TODO: Tests

bool Package::extract(){
	FUN();
	_error.clear();
	std::string _ep = "Failed to extract package " + getFullName() + ": ";

	LOGI("Extracting package " + getFullName() + "...");

	LOGD("Checking database...");
	//Check if the database is ok
	if (_db == nullptr){
		_error = _ep + "Database is not accessible (nullptr)";
		return FAIL(_error);
	}

	//Check the leaf directories
	if (!_db->getCore()->createCacheDirs()){
		_error = _ep + _db->getCore()->getError();
		return FAIL(_error);
	}

	std::string downloadPath = getDownloadPath();
	std::string extractedDir = getExtractedDir();

	std::error_code errCode;
	//If the directory exists, delete it
	if (std::filesystem::exists(extractedDir)){
		LOGD("Destination directory " + extractedDir + " exists, deleting...");
		std::error_code errCode;
		std::filesystem::remove_all(extractedDir, errCode);
		if (errCode){
			_error = _ep + "Failed to delete old extracted directory: " + errCode.message();
			return FAIL(_error);
		}
	}

	//Create the archive instance
	LeafArchive archive;

	//Load the archive
	LOGD("Loading archive " + downloadPath + "...");
	archive.load(downloadPath);
	
	//Extract the archive
	LOGD("Extracting archive " + downloadPath + " into " + extractedDir + "...");
	archive.extract(lConfig.packagesDir());

	return true;
}
