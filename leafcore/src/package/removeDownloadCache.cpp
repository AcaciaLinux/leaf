/**
 * @file		package/removeDownloadCache.cpp
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		The implementation of Package::removeDownloadCache()
 * @copyright	Copyright (c) 2022
 */
#include "log.h"
#include "error.h"
#include "leafdebug.h"
#include "leafconfig.h"

#include "package.h"

#include <filesystem>

//TODO: Tests

void Package::removeDownloadCache(){
	FUN();
	LEAF_DEBUG_EX("Package::removeDownloadCache()");

	namespace fs = std::filesystem;
	std::error_code ec;

	//Check the existance of the file and omit the removal if not
	bool exists = fs::exists(getDownloadPath(), ec);
	if (ec)
		throw new LeafError(Error::FS_ERROR, "In checking existance of " + getDownloadPath(), ec);
	if (!exists){
		LOGD("Download cache file " + getDownloadPath() + " does not exist, skipping removal");
		return;
	}

	fs::remove(getDownloadPath(), ec);
	if (ec)
		throw new LeafError(Error::REMOVEFILE, "Download cache file " + getDownloadPath(), ec);
	
	LOGI("Removed download cache file " + getDownloadPath() + " of " + getFullName());
}
