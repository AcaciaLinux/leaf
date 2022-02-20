/**
 * @file		leafcore/actions.cpp
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		The implementation of the copyDataToRoot()
 * @copyright 	Copyright (c) 2022
 */

#include "log.h"
#include "fail.h"
#include "leafcore.h"
#include "leafconfig.h"
#include "leaffs.h"

#include <filesystem>

bool Leafcore::copyDataToRoot(Package* package){
	namespace fs = std::filesystem;
	FUN();
	if (!checkDirectories())
		return false;

	if (package == nullptr){
		_error = "Failed to copy data: Empty package (nullptr)";
		return FAIL(_error);
	}

	//The error prefix
	std::string _ep = "Failed to copy data of " + package->getFullName() + " to root " + lConfig.rootDir + ": ";

	//The dataDir to copy to the root
	std::string dataDir = package->getExtractedDir() + "/data/";

	//Check if the data directory exists
	if (!fs::exists(dataDir)){
		_error = _ep + "Data directory " + dataDir + " does not exist";
		return FAIL(_error);
	}

	//Check if the root directory exists
	if (!fs::exists(lConfig.rootDir)){
		_error = _ep + "Root directory " + lConfig.rootDir + " does not exist";
		return FAIL(_error);
	}

	//Add index package

	_error = _ep + "Feature not implemented";
	return FAIL(_error);

	return true;
}
