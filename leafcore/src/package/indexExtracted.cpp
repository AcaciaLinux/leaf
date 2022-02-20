/**
 * @file		package/indexExtracted.cpp
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		The code for copying the extracted package to its destination
 * @copyright 	Copyright (c) 2022
 */

#include "log.h"
#include "fail.h"
#include "package.h"
#include "leafconfig.h"
#include "leaffs.h"

#include <filesystem>

bool Package::indexExtracted(){
	FUN();
	_error.clear();
	std::string _ep = "Failed to index package " + getFullName() + " at " + getExtractedDir() + ": ";

	LOGI("Indexing package " + getFullName() + " at " + getExtractedDir());

	if (!std::filesystem::exists(getExtractedDir())){
		_error = _ep + "Package directory " + getExtractedDir() + "does not exist, is the package extracted?";
		return FAIL(_error);
	}

	LeafFS fs(getExtractedDir() + "data/");

	if (!fs.readFiles(false, true)){
		_error = _ep + "Failed to read files: " + fs.getError();
		return FAIL(_error);
	}

	_provided_files = fs.getFiles();

	return true;
}