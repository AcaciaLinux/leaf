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

bool LeafFS::getFiles(std::string prefix, std::string directory, bool recursive){
	FUN();

	fs::directory_iterator dirIt(directory);

	std::string filePath;
	for (fs::directory_entry entry : dirIt){

		filePath = prefix + "/" + entry.path().stem().string() + entry.path().extension().string();

		LOGF("Indexing " + filePath);

		if (entry.is_symlink()){
			//Add the symlink
			_files.push_back(filePath);
		} else if (entry.is_directory()){
			
			//Add the directory
			_directories.push_back(filePath);

			//If recursive operation is nedded, do it
			if (recursive)
				if (!getFiles(filePath, entry.path(), recursive))
					return false;
		} else 
			_files.push_back(filePath);
	}

	return true;
}

bool LeafFS::read(bool recursive){
	FUN();
	_error.clear();

	if (!check()){
		_error = "Failed to get files from " + _curDir + ": " + _error;
		LOGE(_error);
		return false;
	}

	if (!getFiles("", _curDir, recursive)){
		_error = "Failed to get files from " + _curDir + ": " + _error;
		LOGE(_error);
		return false;
	}

	return true;
}

std::string removeFile(std::string path, bool errorOnNotExisting){
	FUN();

	std::error_code ec;

	if (fs::exists(path) || fs::is_symlink(path)){
		
		if (fs::is_directory(path)){
			if (fs::is_empty(path)){
				LOGF("Removing empty directory " + path);
				fs::remove_all(path, ec);
			} else {
				LOGF("Skipping non-empty directory " + path);
			}
		} else {
			LOGF("Removing file " + path);
			fs::remove_all(path, ec);
		}
	} else {
		LOGF("File " + path + " does not exist");
		if (errorOnNotExisting)
			return "File " + path + "does not exist";
	}

	if (ec){
		LOGE("Filesystem error when removing file " + path + ": " + ec.message());
		return "Filesystem error: " + ec.message();
	}

	return "";
}
