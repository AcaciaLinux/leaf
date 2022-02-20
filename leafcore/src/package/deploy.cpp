/**
 * @file		package/deploy.cpp
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		The code for fetching a package from  the server
 * @copyright 	Copyright (c) 2022
 */

#include "log.h"
#include "fail.h"
#include "package.h"
#include "leafconfig.h"

#include <fstream>
#include <filesystem>

bool Package::deploy(){
	FUN();
	_error.clear();
	std::string _ep = "Failed to deploy package " + getFullName() + ": ";

	//Check if the database is ok
	if (_db == nullptr){
		_error = _ep + "Database is not accessible (nullptr)";
		return FAIL(_error);
	}

	if (!_db->getCore()->createConfigDirs()){
		_error = _ep + _db->getCore()->getError();
		return FAIL(_error);
	}

	if (!lConfig.forceOverwrite){
		if (std::filesystem::exists(getInstalledFilePath())){
			_error = _ep + "Leafinstalled file exists, package seems to already be installed";
			return FAIL(_error);
		}
	}
	
	std::ofstream installedFile;
	installedFile.open(getInstalledFilePath(), std::ios::trunc);

	if (!installedFile.is_open()){
		_error = _ep + "Leafinstalled file " + getInstalledFilePath() + " could not be opened for writing";
		installedFile.close();
		return FAIL(_error);
	}

	if (!runPreinstall()){
		_error = _ep + "Running preinstall.sh: " + _error;
		return FAIL(_error);
	}

	if (!copyToRoot()){
		_error = _ep + _error;
		return FAIL(_error);
	}

	if (!runPostinstall()){
		_error = _ep + "Running postinstall.sh: " + _error;
		return FAIL(_error);
	}

	if (!createInstalledFile(installedFile)){
		_error = _ep + _error;
		installedFile.close();
		return FAIL(_error);
	}

	installedFile.close();

	return true;
}
