/**
 * @file		package/deploy.cpp
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		The code for fetching a package from  the server
 * @copyright 	Copyright (c) 2022
 */

#include "log.h"
#include "leafdebug.h"
#include "error.h"
#include "package.h"
#include "leafconfig.h"

#include <fstream>
#include <filesystem>

void Package::deploy(){
	FUN();
	LEAF_DEBUG_EX("Package::deploy()");

	if (_isCollection){
		LOGI("Skipping deployment of collection " + getFullName());
		return;
	}

	//Check if the database is ok
	if (_db == nullptr)
		throw new LeafError(Error::NODB);

	_db->getCore()->createConfigDirs();

	bool overwrite = _db->getCore()->getConfig().forceOverwrite || _db->getCore()->getConfig().force;
	if (std::filesystem::exists(getInstalledFilePath())){
		LOGI("Leafinstalled file for package " + getName() + " exists, reinstalling...");
		overwrite = true;
	}
	
	std::ofstream installedFile;
	installedFile.open(getInstalledFilePath(), std::ios::trunc);

	if (!installedFile.is_open())
		throw new LeafError(Error::OPENFILEW, "Leafinstalled file " + getInstalledFilePath() + " for " + getFullName());

	try {
		
		runPreinstall();
		copyToRoot(overwrite);
		runPostinstall();
		createInstalledFile(installedFile);

	} catch (LeafError* e) {
		std::error_code ec;
		std::filesystem::remove(getInstalledFilePath(), ec);
		if (ec){
			LOGUE("Failed to remove leafinstalled file " + getInstalledFilePath() + " this is FATAL");
		}
		throw e;
	}

	installedFile.close();
} 
