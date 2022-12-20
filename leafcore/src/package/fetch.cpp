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
#include "leaffs.h"

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

	//Check if the package has an expected md5 hash
	if (_remote_md5.length() == 0){
		if (!_db->getCore()->askUserOK("The package " + getFullName() + " does not have an expected MD5 hash, do you want to continue anyway?", false)){
			throw new LeafError(Error::USER_DISAGREE, "Continue installing without MD5 checks");
		}
	}

	if (std::filesystem::exists(getDownloadPath())){
		bool skip = true;

		std::ifstream inFile;
		inFile.open(getDownloadPath(), std::ios::binary);
		if (!inFile.is_open())
			throw new LeafError(Error::OPENFILER, "Download destination for validation " + getDownloadPath());

		std::string existingHash = md5(inFile);

		if (_remote_md5 != existingHash){
			LOGI("[Package][fetch] Existing package file at " + getDownloadPath() + " differs from remote, redownloading...");
			removeFile(getDownloadPath(), true);
			skip = false;
		}

		if (skip){
			LOGI("[Package][fetch] Skipping download of existing validated package file " + getDownloadPath());
			return;
		}
	}


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
	_local_md5 = dl.getMD5();

	outFile.close();

	//Check the md5 hash
	if (_remote_md5.length() != 0 && _remote_md5 != _local_md5){

		LOGUE(
			"The package " + getFullName()  + " has an invalid hash: \n" + 
			"----------------------------------------\n" +
			"remote: " + _remote_md5 + "\n" +
			"local:  " + _local_md5 + "\n" +
			"----------------------------------------");

		if (!_db->getCore()->askUserOK("It might be corrupted, do you want to continue anyway?", false)){

			{//Remove the corrupted file
				LOGF("[Package][fetch] Removing corrupted package file " + getDownloadPath());
				std::error_code ec;
				std::filesystem::remove_all(getDownloadPath(), ec);

				if (ec){
					throw new LeafError(Error::REMOVEFILE, ec);
				}
			}

			throw new LeafError(Error::USER_DISAGREE, "Continue installing a package with invalid MD5 hash");
		}
	}

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
