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
	_error.clear();
	if (!checkDirectories())
		return false;

	std::string pkgListFilePath = lConfig.pkgListPath();
	LOGI("Fetching package list from " + _pkglistURL + " to " + pkgListFilePath);
	//Create the output file stream and check it
	std::ofstream file;
	file.open(pkgListFilePath, std::ios::trunc);

	if (!file.is_open()){
		_error = "Failed to open " + pkgListFilePath + " for writing";
		LOGE("Package list update: " + _error);
		file.close();
		return false;
	}

	//Create the downloader instance and download the file
	Downloader dl;
	if (!dl.init()){
		_error = "Failed to initialize downloader: " + dl.getError();
		LOGE("Package list update: " + _error);
		return false;
	}
	if (!dl.download(_pkglistURL, file)){
		_error = "Failed to download from " + _pkglistURL + ": " + dl.getError();
		LOGE("Package list update: " + _error);		
		return false;
	}
	
	file.close();

	return true;
}
