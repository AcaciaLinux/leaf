/**
 * @file		package/fetch.cpp
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		The code for fetching a package from  the server
 * @copyright 	Copyright (c) 2022
 */

#include "log.h"
#include "leafdebug.h"
#include "package.h"
#include "downloader.h"

#include <fstream>
#include <filesystem>

void Package::fetch(){
	FUN();
	LEAF_DEBUG_EX("Package::fetch()");

	LOGI("Fetching package " + getFullName());

	if (_isCollection){
		LOGI("Skipping fetch of collection " + getFullName());
		return;
	}

	//Check if the database is ok
	if (_db == nullptr)
		throw new LeafError(Error::NODB);

	//Check the leaf directories
	_db->getCore()->createCacheDirs();

	std::string destination = getDownloadPath();
	
	if (destination.empty())
		throw new LeafError(Error::PACKAGE_FETCH_DEST_EMPTY, getFullName());

	//Create the downloader instance
	Downloader dl;
	dl.init();

	LOGD("Opening destination file " + destination + "...");
	//Create and open the destination file
	std::ofstream outFile;
	outFile.open(destination, std::ios::trunc | std::ios::binary);

	//Check if the destination file is open
	if (!outFile.is_open())
		throw new LeafError(Error::OPENFILEW, "Download for " + getFullName() + ": " + destination);

	LOGI("Downloading package " + getFullName() + " to " + destination);
	
	//Download the package file
	dl.download(getFetchURL(), outFile);

	outFile.close();
}
