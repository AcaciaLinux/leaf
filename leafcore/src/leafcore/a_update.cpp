/**
 * @file		leafcore/a_update.cpp
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		The implementation of Leafcore::a_update()
 * @copyright	Copyright (c) 2022
 */
#include "log.h"
#include "leafdebug.h"
#include "leafconfig.h"

#include "leafcore.h"
#include "downloader.h"
#include "branchmaster.h"

#include <fstream>

void Leafcore::a_update(){
	FUN();
	LEAF_DEBUG_EX("Leafcore::a_update()");

	checkDirectories();

	std::string pkgListFilePath = _config.pkgListPath();
	LOGI("Fetching package list from " + _config.pkgListURL + " to " + pkgListFilePath);
	//Create the output file stream and check it
	std::ofstream outFile;
	outFile.open(pkgListFilePath, std::ios::trunc);

	if (!outFile.is_open()){
		outFile.close();
		throw new LeafError(Error::OPENFILEW, pkgListFilePath);
	}

	//Create the downloader instance and download the file
	Downloader dl;
	dl.init();
	size_t dRes = dl.download(_config.pkgListURL, outFile);
	outFile.close();

	//If everything is ok, return
	if (dRes < 400)
		return;


	std::ifstream ecFile(pkgListFilePath, std::ios::in);
	if (!ecFile.is_open()){
		ecFile.close();
		throw new LeafError(Error::OPENFILER, pkgListFilePath + "for reading back branchmaster error code");
	}

	std::string resString;
	if (!getline(ecFile, resString)){
		#warning Handle this error
	}

	BranchMaster::ec ec = BranchMaster::parseEC(dRes, resString);


	if (ec == BranchMaster::E_NONE)
		LOGUW("The package list fetch resulted in a HTTP error code, but the code parsing resulted in no error, something suspicous could be going on!");
	else
		throw new LeafError(Error::BRANCHMASTER_ERROR, getBranchMasterECString(ec));
}
