/**
 * @file		leafcore/parsePackageList.cpp
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		The implementation of parsePackageList()
 * @copyright	Copyright (c) 2022
 */
#include "log.h"
#include "leafdebug.h"
#include "leafconfig.h"

#include "leafcore.h"
#include "pkglistparser.h"

#include <fstream>

void Leafcore::parsePackageList(){
	FUN();
	parsePackageList(_config.pkgListPath());
}

void Leafcore::parsePackageList(std::string path){
	FUN();
	LEAF_DEBUG_EX("Leafcore::parsePackageList()");

	checkDirectories();

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
	parser.parse(file);

	file.close();

	//Try to apply the file to the database
	parser.applyToDB(*_packageListDB);

	LOGI("Done parsing package list");
	_loadedPkgList = true;
}
