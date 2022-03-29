/**
 * @file		leafcore/a_update.cpp
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		The implementation of Leafcore::a_update()
 * @copyright	Copyright (c) 2022
 */
#include "log.h"
#include "error.h"
#include "leafdebug.h"
#include "leafconfig.h"

#include "leafcore.h"
#include "downloader.h"

#include <fstream>

bool Leafcore::a_update(){
	FUN();

	LEAF_DEBUG("Leafcore::a_update()");

	//TODO: exceptions

	_error.clear();
	if (!checkDirectories())
		return false;

	std::string pkgListFilePath = lConfig.pkgListPath();
	LOGI("Fetching package list from " + _pkglistURL + " to " + pkgListFilePath);
	//Create the output file stream and check it
	std::ofstream file;
	file.open(pkgListFilePath, std::ios::trunc);

	if (!file.is_open()){
		file.close();
		throw new LeafError(Error::OPENFILEW, pkgListFilePath);
	}

	//Create the downloader instance and download the file
	Downloader dl;
	dl.init();
	dl.download(_pkglistURL, file);
	
	file.close();

	return true;
}
