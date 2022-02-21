/**
 * @file		package/removeFromRoot.cpp
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

bool Package::removeFromRoot(){
	FUN();
	_error.clear();
	std::string _ep = "Failed to remove package " + getFullName() + ": ";

	//Check if the database is ok
	if (_db == nullptr){
		_error = _ep + "Database is not accessible (nullptr)";
		return FAIL(_error);
	}

	std::error_code ec;
	for (std::string file : _provided_files){
		if (std::filesystem::is_directory(lConfig.rootDir + file)){
			LOGD("Skipping " + lConfig.rootDir + file);
			continue;
		}

		std::filesystem::remove(lConfig.rootDir + file, ec);

		if (ec){
			_error = _ep + "Filesystem error: " + ec.message() + " when processing " + lConfig.rootDir + file;
			return FAIL(_error);
		}
	}

	_error = _ep + "Feature not implemented";
	return FAIL(_error);

	return true;
}
