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

void Package::clear(){
	FUN();

	_name.clear();
	_description.clear();
	_versionString.clear();
	_dependencies.clear();
	_fetchURL.clear();
	_provided_files.clear();
}
