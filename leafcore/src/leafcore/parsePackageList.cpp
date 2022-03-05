/**
 * @file		leafcore/parsePackageList.cpp
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		The implementation of parsePackageList()
 * @copyright	Copyright (c) 2022
 */
#include "log.h"
#include "error.h"
#include "leafdebug.h"
#include "leafconfig.h"

#include "leafcore.h"
#include "pkglistparser.h"

#include <fstream>

bool Leafcore::parsePackageList(){
	FUN();
	return parsePackageList(lConfig.pkgListPath());
}

bool Leafcore::parsePackageList(std::string path){
	FUN();

	LEAF_DEBUG("Leafcore::parsePackageList()");

	//TODO: exceptions

	_error.clear();
	if (!checkDirectories())
		return false;

	if (_packageListDB == nullptr)
		throw new LeafError(Error::NODB, "Package list");

	_packageListDB->clear();

	_pkglistURL = path;

	LOGI("Parsing package list " + path);

	std::ifstream file;
	file.open(path, std::ios::in);

	//Try opening package list file
	if (!file.is_open()){
		file.close();
		throw new LeafError(Error::OPENFILEW, "Package list, try \"leaf update\" to fix this");		
	}

	PackageListParser parser;

	//Try parsing the file
	if (!parser.parse(file)){
		_error = "Parser error: " + parser.getError();
		LOGE("Failed to parse package list with " + _error);
		file.close();
		return false;
	}

	file.close();

	//Try to apply the file to the database
	if (!parser.applyToDB(*_packageListDB)){
		_error = "Parser apply error: " + parser.getError();
		LOGE("Failed to apply package list with " + _error);
		return false;
	}

	LOGI("Done parsing package list");
	_loadedPkgList = true;

	return true;
}
