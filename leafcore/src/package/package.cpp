/**
 * @file		package/package.cpp
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		The core implementation of Package
 * @copyright 	Copyright (c) 2022
 */

#include "log.h"
#include "package.h"

#include <algorithm>
#include <stdint.h>

Package::Package(std::string name, std::string version){
	FUN();
	_name = name;
	_versionString = version;
	LOGF("Created package \"" + getFullName() + "\"");
}

void Package::addProvidedFile(std::string filepath){
	FUN();
	{//Strip the path to remove leading "/"
		std::string msg = "Stripping \"" + filepath + "\" to \"";

		while(filepath.length() > 0 && filepath[0] == '/')
			filepath = filepath.substr(1);

		LOGF(msg + filepath + "\"");
	}

	_provided_files.push_back(filepath);
}

bool Package::checkFileProvided(std::string filepath){
	FUN();

	{//Strip the path to remove leading "/"
		std::string msg = "Stripping \"" + filepath + "\" to \"";

		while(filepath.length() > 0 && filepath[0] == '/')
			filepath = filepath.substr(1);

		LOGF(msg + filepath + "\"");
	}

	std::deque<std::string>::iterator results = std::find(_provided_files.begin(), _provided_files.end(), filepath);
	return _provided_files.end() != results;
}

bool Package::createInstalledFile(std::ostream& out){
	FUN();

	if (!out.good())
		return false;

	out << _name << std::endl;
	out << _versionString << std::endl;
	out << std::to_string(_dependencies.size()) << std::endl;
	out << std::to_string(_provided_files.size() + _provided_directories.size()) << std::endl;

	for (std::string dep : _dependencies)
		out << dep << std::endl;

	for (std::string file : _provided_files)
		out << file << std::endl;

	for (auto rit = _provided_directories.rbegin(); rit != _provided_directories.rend(); ++rit){
		out << *rit << std::endl;
	}

	return true;
}

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

void Package::clear(){
	FUN();

	_name.clear();
	_description.clear();
	_versionString.clear();
	_dependencies.clear();
	_fetchURL.clear();
	_provided_files.clear();
}
