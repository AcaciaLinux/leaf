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
#include "md5.h"

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

	LOGD("Opening destination file " + destination + "...");
	//Create and open the destination file
	std::ofstream outFile;
	outFile.open(destination, std::ios::trunc | std::ios::binary);

	//Check if the destination file is open
	if (!outFile.is_open())
		throw new LeafError(Error::OPENFILEW, "Download for " + getFullName() + ": " + destination);

	//Create the downloader instance
	Downloader dl(getFetchURL(), outFile, _db->getCore()->getConfig().noProgress);
	dl.init();

	LOGI("Downloading package " + getFullName() + " to " + destination);
	
	//Download the package file
	if (_db->getCore()->getConfig().noProgress)
		LOGU("Downloading package " + getFullName() + "...");
	size_t dRes = dl.download("Downloading " + getFullName());
	outFile.close();

	//If everything is ok, return
	if (dRes < 400)
		return;


	std::ifstream ecFile(destination, std::ios::in);
	if (!ecFile.is_open()){
		ecFile.close();
		throw new LeafError(Error::OPENFILER, destination + "for reading back branchmaster error code");
	}

	std::string resString;
	if (!getline(ecFile, resString)){
		ecFile.close();
		throw new LeafError(Error::OPENFILER, destination + "while reading back branchmaster error code");
	}

	BranchMaster::ec ec = BranchMaster::parseEC(dRes, resString);


	if (ec == BranchMaster::E_NONE)
		LOGUW("The package fetch resulted in a HTTP error code, but the code parsing resulted in no error, something suspicous could be going on!");
	else
		throw new LeafError(Error::BRANCHMASTER_ERROR, BranchMaster::getECString(ec));
}
