/**
 * @file		package/fetch.cpp
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		The code for fetching a package from  the server
 * @copyright 	Copyright (c) 2022
 */

#include "log.h"
#include "fail.h"
#include "package.h"
#include "downloader.h"

#include <fstream>

bool Package::fetch(){
	FUN();
	_error.clear();
	std::string _ep = "Failed to fetch package " + getFullName() + ": ";

	LOGI("Fetching package " + getFullName());

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

	std::string destination = getDownloadPath();
	
	if (destination.empty()){
		_error = _ep + "Destination directory: " + _error;
		return FAIL(_error);
	}

	//Create the downloader instance
	Downloader dl;

	if (!dl.init()){
		_error = _ep + "Failed to initialize downloader: " + dl.getError();
		return FAIL(_error);
	}

	//Create and open the destination file
	std::ofstream outFile;
	outFile.open(destination, std::ios::trunc | std::ios::binary);

	if (!outFile.is_open()){
		_error = _ep + "Could not open destination file " + destination + " for writing";
		outFile.close();
		return FAIL(_error);
	}

	LOGI("Downloading package " + getFullName() + " to " + destination);
	
	if (!dl.download(getFetchURL(), outFile)){
		_error = _ep + "Download from " + getFetchURL() + " failed: " + dl.getError();
		outFile.close();
		return FAIL(_error);
	}

	outFile.close();

	return true;
}
