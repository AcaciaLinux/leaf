/**
 * @file		package/deploy.cpp
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		The code for fetching a package from  the server
 * @copyright 	Copyright (c) 2022
 */

#include "log.h"
#include "fail.h"
#include "error.h"
#include "package.h"
#include "leafconfig.h"

#include <fstream>
#include <filesystem>

bool Package::deploy(){
	FUN();
	_error.clear();
	std::string _ep = "Failed to deploy package " + getFullName() + ": ";

	if (_isCollection){
		LOGI("Skipping deployment of collection " + getFullName());
		return true;
	}

	//Check if the database is ok
	if (_db == nullptr){
		throw new LeafError(Error::NODB);
		_errorCode = EC::Package::NODB;
		_error = _ep + "Database is not accessible (nullptr)";
		return FAIL(_error);
	}

	if (!_db->getCore()->createConfigDirs()){
		_errorCode = EC::Package::NOCONFDIR;
		_error = _ep + _db->getCore()->getError();
		return FAIL(_error);
	}

	bool overwrite = false;
	if (std::filesystem::exists(getInstalledFilePath())){
		LOGI("Leafinstalled file for package " + getName() + " exists, reinstalling...");
		overwrite = true;
	}
	
	std::ofstream installedFile;
	installedFile.open(getInstalledFilePath(), std::ios::trunc);

	if (!installedFile.is_open()){
		_errorCode = EC::Package::DEPLOY_OPENINSTALLEDFILE;
		_error = _ep + "Leafinstalled file " + getInstalledFilePath() + " could not be opened for writing";
		installedFile.close();
		return FAIL(_error);
	}

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

	return true;
} 
