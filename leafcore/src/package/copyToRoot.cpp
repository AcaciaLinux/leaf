/**
 * @file		package/copyToRoot.cpp
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		The code for copying the extracted package to its destination
 * @copyright 	Copyright (c) 2022
 */

#include "log.h"
#include "fail.h"
#include "package.h"
#include "leafconfig.h"

#include <filesystem>

bool Package::copyToRoot(){
	FUN();
	_error.clear();
	std::string _ep = "Failed to copy package " + getFullName() + " to root " + lConfig.rootDir + ": ";

	LOGI("Copying package " + getFullName() + " to root " + lConfig.rootDir);

	namespace fs = std::filesystem;

	if (!fs::exists(lConfig.rootDir)){
		_error = _ep + "Root directory " + lConfig.rootDir + " does not exist";
		return FAIL(_error);
	}

	if (!fs::exists(getExtractedDir())){
		_error = _ep + "Package directory " + getExtractedDir() + "does not exist, is the package extracted?";
		return FAIL(_error);
	}

	if (!indexExtracted()){
		_error = _ep + "Could not index: " + _error;
		return FAIL(_error);
	}

	//If leaf should not overwrite existing files, check for them
	if (!lConfig.forceOverwrite){
		//Got through every entry
		for (std::string file : _provided_files){
			
			//Directories can be overwritten
			if (!std::filesystem::is_directory(lConfig.rootDir + file)){

				//If the file exists, error out
				if (std::filesystem::exists(lConfig.rootDir + file)){
					_error = _ep + "File does already exist: " + lConfig.rootDir + file;
					return FAIL(_error);
				}
			}
		}
	}

	//Finally, copy the package
	std::error_code ec;

	fs::copy_options options = fs::copy_options::copy_symlinks;

	if (lConfig.forceOverwrite)
		options |= fs::copy_options::update_existing;

	std::string dataDir = getExtractedDir() + "data/";
	std::string destDir = lConfig.rootDir;

	for (std::string file : _provided_files){

		if (fs::is_directory(dataDir + file)){
			LOGF("Creating directory " + destDir + file);
			fs::create_directories(destDir + file, ec);
		} else {
			LOGF("Copying " + dataDir + file + " -> " + destDir + file);
			fs::copy(dataDir + file, destDir + file, options, ec);
		}

		if (ec){
			_error = _ep + "Copy " + destDir + file + " failed: " + ec.message() + " code: " + std::to_string(ec.value());
			return FAIL(_error);
		}
	}

	return true;
}
