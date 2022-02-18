/**
 * @file		leafcore/parsers.cpp
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		The parsers of Leafcore
 * @copyright 	Copyright (c) 2022
 */

#include "log.h"
#include "leafcore.h"

#include "pkglistparser.h"
#include "leaffs.h"

#include <fstream>
#include <string>

bool Leafcore::parsePackageList(std::string path){
	FUN();
	_error.clear();
	if (!checkDirectories())
		return false;

	_packageListDB->clear();

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
	if (!parser.applyToDB(*_packageListDB)){
		_error = "Parser apply error: " + parser.getError();
		LOGE("Failed to apply package list with " + _error);
		return false;
	}

	LOGI("Done parsing package list");
	_loadedPkgList = true;

	return true;
}

bool Leafcore::parseInstalled(){
	FUN();
	_error.clear();
	if (!checkDirectories())
		return false;

	_installedDB->clear();

	std::deque<std::string> installedFiles;

	{	//Read the directory
		LeafFS installedDir("/etc/leaf/installed/");

		if (!installedDir.check()){
			_error = "Failed to parse installed packages: " + installedDir.getError();
			LOGE(_error);
			return false;
		}

		if (!installedDir.readFiles(true, false)){
			_error = "Failed to parse installed packages: " + installedDir.getError();
			LOGE(_error);
			return false;
		}

		installedFiles = installedDir.getFiles();
	}

	if (installedFiles.size() == 0){
		LOGW("It seems that no packages are installed on the system");
		return true;
	}

	LOGD("Installed packages: ");
	for (std::string file : installedFiles){
		file.erase(0, 1);
		LOGD(" -> " + file);

		Package* newPack = _installedDB->newPackage("", "");

		std::ifstream inFile;
		inFile.open("/etc/leaf/installed/" + file + ".leafinstalled", std::ios::in);

		if (!inFile.is_open()){
			_error = "Failed to parse installed package " + file + ", failed to open file";
			LOGE(_error);
			return false;
		}

		if (!newPack->parseInstalledFile(inFile)){
			_error = "Failed to parse installed package " + file + ": " + newPack->getError();
			LOGE(_error);
			inFile.close();
			return false;
		}

		inFile.close();
	}

	return true;
}