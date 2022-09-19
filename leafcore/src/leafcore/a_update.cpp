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

#include <fstream>

void Leafcore::a_update(){
	FUN();
	LEAF_DEBUG_EX("Leafcore::a_update()");

	checkDirectories();

	std::string pkgListFilePath = _config.pkgListPath();
	LOGI("Fetching package list from " + _config.pkgListURL + " to " + pkgListFilePath);
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
	dl.download(_config.pkgListURL, file);
	
	file.close();
}
