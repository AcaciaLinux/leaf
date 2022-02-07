/**
 * @file		leafcore/leafcore.cpp
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		The core implementation of Leafcore
 * @copyright 	Copyright (c) 2022
 */

#include "log.h"
#include "leafcore.h"

#include "pkglistparser.h"
#include "downloader.h"

#include <fstream>

Leafcore::Leafcore(std::string rootPath){
	FUN();

	this->_rootPath = rootPath;
}

bool Leafcore::parsePackageList(){
	FUN();
	return parsePackageList(_pkglistFile);
}

bool Leafcore::parsePackageList(std::string path){
	FUN();
	_error = "";

	_pkglistURL = path;

	LOGI("Parsing package list " + path);

	std::ifstream file;
	file.open(path, std::ios::in);

	//Try opening package list file
	if (!file.is_open()){
		_error = "File " + path + " could not be opened";
		LOGE("Could not open package list " + path);
		file.close();
		return false;
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
	if (!parser.applyToDB(_db)){
		_error = "Parser apply error: " + parser.getError();
		LOGE("Failed to apply package list with " + _error);
		return false;
	}

	LOGI("Done parsing package list");

	return true;
}

bool Leafcore::a_update(){
	FUN();
	_error = "";

	LOGI("Fetching package list from " + _pkglistURL + " to " + _pkglistFile);

	//Create the output file stream and check it
	std::ofstream file;
	file.open(_pkglistFile, std::ios::out);

	if (!file.is_open()){
		_error = "Failed to open " + _pkglistFile + " for writing";
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
