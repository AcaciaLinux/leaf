/**
 * @file		leafcore/parseInstalled.cpp
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		The implementation of parseInstalled()
 * @copyright	Copyright (c) 2022
 */
#include "log.h"
#include "error.h"
#include "leafdebug.h"
#include "leafconfig.h"

#include "leafcore.h"
#include "leaffs.h"

#include "fail.h"

#include <filesystem>
#include <fstream>

bool Leafcore::parseInstalled(){
	FUN();

	LEAF_DEBUG("Leafcore::parseInstalled()");

	//TODO: exception

	_error.clear();
	if (!checkDirectories())
		return false;

	if (_installedDB == nullptr)
		throw new LeafError(Error::NODB, "Installed db");

	_installedDB->clear();

	std::deque<std::string> installedFiles;

	createConfigDirs();

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
			inFile.close();
			throw new LeafError(Error::OPENFILER, "Installed file " + file);
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