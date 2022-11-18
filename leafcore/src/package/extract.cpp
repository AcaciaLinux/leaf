/**
 * @file		package/extract.cpp
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		The code for extracting a downloaded package for further processing
 * @copyright 	Copyright (c) 2022
 */

#include "log.h"
#include "leafdebug.h"
#include "package.h"
#include "leafarchive.h"
#include "leafconfig.h"

#include <filesystem>

//TODO: Tests

void Package::extract(){
	FUN();
	LEAF_DEBUG_EX("Package::extract()");

	LOGI("Extracting package " + getFullName() + "...");

	if (_isCollection){
		LOGI("Skipping extracting of collection " + getFullName());
		return;
	}

	LOGD("Checking database...");
	//Check if the database is ok
	if (_db == nullptr)
		throw new LeafError(Error::NODB);

	//Check the leaf directories
	_db->getCore()->createCacheDirs();

	std::string sourcePath;
	if (_isLocal)
		sourcePath = _localSourcePath;
	else
		sourcePath = getDownloadPath();
	
	std::string extractedDir = getExtractedDir();

	std::error_code errCode;
	//If the directory exists, delete it
	if (std::filesystem::exists(extractedDir)){
		LOGD("Destination directory " + extractedDir + " exists, deleting...");
		std::error_code errCode;
		std::filesystem::remove_all(extractedDir, errCode);
		if (errCode)
			throw new LeafError(Error::REMOVEDIR, extractedDir);
	}

	//Create the archive instance
	LeafArchive archive;

	//Load the archive
	LOGD("Loading archive " + sourcePath + "...");
	archive.load(sourcePath);
	
	//Extract the archive
	LOGD("Extracting archive " + sourcePath + " into " + extractedDir + "...");
	archive.extract(_db->getCore()->getConfig().packagesDir());
}
