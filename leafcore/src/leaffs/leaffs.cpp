/**
 * @file		leaffs/leaffs.cpp
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		The core implementation of LeafFS
 * @copyright 	Copyright (c) 2022
 */

#include "log.h"
#include "leaffs.h"

#include <filesystem>

namespace fs = std::filesystem;

LeafFS::LeafFS(std::string dir){
	FUN();
	
	_curDir = dir;
}

bool LeafFS::check(){
	FUN();
	_error.clear();

	if (_curDir.empty()){
		_error = "Directory string is empty";
		LOGE("Check failed: " + _error);
		return false;
	}

	if (!fs::exists(_curDir)){
		_error = _curDir + " does not exist";
		LOGE("Check of " + _curDir + " failed: " + _error);
		return false;
	}

	if (!fs::is_directory(_curDir)){
		_error = _curDir + " is not a directory";
		LOGE("Check of " + _curDir + " failed: " + _error);
		return false;
	}

	return true;
}

bool LeafFS::getFiles(std::string prefix, std::string directory, bool recursive, bool filesonly){
	FUN();

	fs::directory_iterator dirIt(directory);

	std::string filePath;
	for (fs::directory_entry entry : dirIt){
		//if (prefix[prefix.size()-1] != '/')
		//	prefix += '/';

		filePath = prefix + "/" + entry.path().stem().string() + entry.path().extension().string();

		if (entry.is_directory()){
			
			//If we are allowed to add directories, add it
			if (!filesonly)
				_files.push_back(filePath);

			//If recursive operation is nedded, do it
			if (recursive)
				if (!getFiles(filePath, entry.path(), recursive, filesonly))
					return false;
		} else 
			_files.push_back(filePath);
	}

	return true;
}

bool LeafFS::readFiles(bool onlyFiles, bool recursive){
	FUN();
	_error.clear();

	if (!check()){
		_error = "Failed to get files from " + _curDir + ": " + _error;
		LOGE(_error);
		return false;
	}

	if (!getFiles("", _curDir, recursive, onlyFiles)){
		_error = "Failed to get files from " + _curDir + ": " + _error;
		LOGE(_error);
		return false;
	}

	return true;
}
