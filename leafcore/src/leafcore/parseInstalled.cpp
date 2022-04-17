/**
 * @file		leafcore/parseInstalled.cpp
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		The implementation of parseInstalled()
 * @copyright	Copyright (c) 2022
 */
#include "log.h"
#include "leafdebug.h"
#include "leafconfig.h"

#include "leafcore.h"
#include "leaffs.h"

#include <filesystem>
#include <fstream>

void Leafcore::parseInstalled(){
	FUN();
	LEAF_DEBUG_EX("Leafcore::parseInstalled()");

	checkDirectories();

	if (_installedDB == nullptr)
		throw new LeafError(Error::NODB, "Installed db");

	_installedDB->clear();

	std::deque<std::string> installedFiles;

	createConfigDirs();

	{	//Read the directory
		LeafFS installedDirFS(lConfig.installedDir());
	
		installedDirFS.check();

		installedDirFS.read(true);

		installedFiles = installedDirFS.getFiles();
	}

	if (installedFiles.size() == 0){
		LOGW("It seems that no packages are installed on the system");
		return;
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

		newPack->parseInstalledFile(inFile);

		_installedDB->renamePackage(file, newPack->getName());

		LOGD(" -> " + newPack->getName());

		inFile.close();
	}
}
