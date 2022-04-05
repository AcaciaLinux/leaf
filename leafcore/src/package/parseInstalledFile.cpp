/**
 * @file		package/parseInstalledFile.cpp
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		The implementation of Package::parseInstalledFile()
 * @copyright	Copyright (c) 2022
 */
#include "log.h"
#include "error.h"
#include "leafdebug.h"
#include "leafconfig.h"
#include "package.h"

bool Package::parseInstalledFile(std::istream& in){
	FUN();
	_error.clear();

	if (!in.good()){
		_error = "Stream seems bad";
		LOGE("Failed to parse .leafinstalled file: " + _error);
		clear();
		return false;
	}

	std::string line;

	{	//Parse name and version string
		if (!getline(in, line)){
			_error = "Uexpected EOF for name";
			LOGE("Failed to parse .leafinstalled file: " + _error);
			clear();
			return false;
		}
		_name = line;

		if (!getline(in, line)){
			_error = "Uexpected EOF for version";
			LOGE("Failed to parse .leafinstalled file: " + _error);
			clear();
			return false;
		}
		_versionString = line;
	}
	
	int countDependencies = 0;
	int countFiles = 0;

	{	//Parse dependency and file count
		if (!getline(in, line)){
			_error = "Uexpected EOF for dependency count";
			LOGE("Failed to parse .leafinstalled file: " + _error);
			clear();
			return false;
		}
		
		try{
			countDependencies = stoi(line);
		} catch (...){
			_error = "Error parsing dependency count string to integer: " + line;
			LOGE("Failed to parse .leafinstalled file: " + _error);
			clear();
			return false;
		}
		
		if (!getline(in, line)){
			_error = "Uexpected EOF for file count";
			LOGE("Failed to parse .leafinstalled file: " + _error);
			clear();
			return false;
		}
		
		try{
			countFiles = stoi(line);
		} catch (...){
			_error = "Error parsing file count string to integer: " + line;
			LOGE("Failed to parse .leafinstalled file: " + _error);
			clear();
			return false;
		}
	}

	{	//Parse dependencies
		for (int i = 0; i < countDependencies; i++){
			if (!getline(in, line)){
				_error = "Uexpected EOF for dependency " + std::to_string(i+1) + "/" + std::to_string(countDependencies);
				LOGE("Failed to parse .leafinstalled file: " + _error);
				clear();
				return false;
			}

			_dependencies.push_back(line);
		}
	}

	{	//Parse files
		for (int i = 0; i < countFiles; i++){
			if (!getline(in, line)){
				_error = "Uexpected EOF for file #" + std::to_string(i) + "/" + std::to_string(countFiles);
				LOGE("Failed to parse .leafinstalled file: " + _error);
				clear();
				return false;
			}

			_provided_files.push_back(line);
		}
	}
	
	return true;
}
