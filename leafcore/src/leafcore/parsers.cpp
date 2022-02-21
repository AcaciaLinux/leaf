/**
 * @file		leafcore/parsers.cpp
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		The parsers of Leafcore
 * @copyright 	Copyright (c) 2022
 */

#include "log.h"
#include "fail.h"
#include "leafconfig.h"
#include "leafcore.h"

#include "pkglistparser.h"
#include "leaffs.h"

#include <fstream>
#include <string>
#include <filesystem>

bool Leafcore::parsePackageList(){
	FUN();
	return parsePackageList(lConfig.pkgListPath());
}

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
		_error = "Failed to read package list, try \"leaf update\" to fix this";
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

	if (!std::filesystem::exists(lConfig.installedDir())){
		std::error_code ec;
		std::filesystem::create_directories(lConfig.installedDir(), ec);

		if (ec){
			_error = "Failed to create installed directory " + lConfig.installedDir() + ": " + ec.message();
			return FAIL(_error);
		}
	}

	{	//Read the directory
		LeafFS installedDirFS(lConfig.installedDir());

		if (!installedDirFS.check()){
			_error = "Failed to parse installed packages: " + installedDirFS.getError();
			return FAIL(_error);
		}

		if (!installedDirFS.read(true)){
			_error = "Failed to parse installed packages: " + installedDirFS.getError();
			return FAIL(_error);
		}

		installedFiles = installedDirFS.getFiles();
	}

	if (installedFiles.size() == 0){
		LOGW("It seems that no packages are installed on the system");
		return true;
	}

	LOGD("Installed packages: ");
	for (std::string file : installedFiles){
		file.erase(0, 1);

		Package* newPack = _installedDB->newPackage(file, "");

		std::ifstream inFile;
		inFile.open(lConfig.installedDir() + file, std::ios::in);

		if (!inFile.is_open()){
			_error = "Failed to parse installed package " + file + ", failed to open file " + lConfig.installedDir() + file;
			return FAIL(_error);
		}

		if (!newPack->parseInstalledFile(inFile)){
			_error = "Failed to parse installed package " + file + ": " + newPack->getError();
			inFile.close();
			return FAIL(_error);
		}

		_installedDB->renamePackage(file, newPack->getName());

		LOGD(" -> " + newPack->getName());

		inFile.close();
	}

	return true;
}