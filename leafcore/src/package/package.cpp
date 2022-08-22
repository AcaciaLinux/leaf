/**
 * @file		package/package.cpp
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		The core implementation of Package
 * @copyright 	Copyright (c) 2022
 */

#include "log.h"
#include "error.h"
#include "package.h"

#include <algorithm>
#include <stdint.h>
#include <filesystem>

Package::Package(){
	FUN();
	LOGF("Created new unknown package");
}

Package::Package(std::string name, std::string version){
	FUN();
	_name = name;
	_versionString = version;
	LOGF("Created package \"" + getFullName() + "\"");
}

Package* Package::CreateFromLocal(std::string path){
	FUN();
	LOGF("Creating new local package from path \"" + path + "\"");

	using namespace std;
	std::filesystem::path fsPath(path);

	//Check if the file even exists
	{
		std::error_code ec;
		bool exists = std::filesystem::exists(path, ec);

		if (ec)
			throw new LeafError(Error::FS_ERROR, "Checking if " + path + " exists: ", ec);
		
		if (!exists)
			throw new LeafError(Error::PKG_NOTEXISTING, path);
	}

	//Check if the package name ends in .lfpkg
	if (fsPath.extension() != ".lfpkg")
		throw new LeafError(Error::PKG_NOTLFPKG, path);

	//Create the name of the package
	std::string fileName = fsPath.filename().string();
	fileName = fileName.substr(0, fileName.find(".lfpkg"));

	//Check if there is at least one '-' to separate the version
	size_t delimiterPos = fileName.rfind('-');
	if (delimiterPos == std::string::npos)
		throw new LeafError(Error::PKG_NODELIMITER, path);

	Package* newPackage = new Package();
	newPackage->_isLocal = true;

	newPackage->_name = fileName.substr(0, delimiterPos);
	newPackage->_versionString = fileName.substr(delimiterPos+1);
	
	return newPackage;
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
