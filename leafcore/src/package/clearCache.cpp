/**
 * @file		package/clearcache.cpp
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		The implementation of Package::clearCache()
 * @copyright	Copyright (c) 2022
 */
#include "log.h"
#include "error.h"
#include "leafdebug.h"
#include "leafconfig.h"

#include "package.h"

#include <filesystem>

bool Package::clearCache(){
	FUN();

	//TODO: test this function
	LEAF_DEBUG_EX("Package::clearCache()");

	if (_isCollection){
		LOGI("Skipping cache clearing of collection " + getFullName());
		return true;
	}

	std::string extractedDir = getExtractedDir();

	//If the cache directory exists, delete it
	if (std::filesystem::exists(extractedDir)){
		LOGI("Deleting cache at " + extractedDir + " of package " + getFullName() + "...");
		std::error_code errCode;
		std::filesystem::remove_all(extractedDir, errCode);
		if (errCode){
			throw new LeafError(Error::REMOVEDIR, extractedDir, errCode);
		}
	} else {
		LOGI("Cache directory " + extractedDir + " of package " + getFullName() + " did not exist");
	}

	return true;
}
