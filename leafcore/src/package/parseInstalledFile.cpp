/**
 * @file		package/parseInstalledFile.cpp
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		The implementation of Package::parseInstalledFile()
 * @copyright	Copyright (c) 2022
 */
#include "log.h"
#include "leafdebug.h"
#include "leafconfig.h"
#include "package.h"

void Package::parseInstalledFile(std::istream& in){
	FUN();

	LEAF_DEBUG_EX("Package::parseInstalledFile()");

	//TODO: tests

	if (!in.good())
		throw new LeafError(Error::BAD_ISTREAM, "parseInstalledFile() (This is a bug)");

	std::string line;

	{	//Parse name and version string
		if (!getline(in, line))
			throw new LeafError(Error::PACKAGE_UNEXPECTED_EOF, "name");
		_name = line;

		if (!getline(in, line))
			throw new LeafError(Error::PACKAGE_UNEXPECTED_EOF, "version");
		_versionString = line;
	}
	
	int countDependencies = 0;
	int countFiles = 0;

	{	//Parse dependency and file count
		if (!getline(in, line))
			throw new LeafError(Error::PACKAGE_UNEXPECTED_EOF, "dependency count");
		try{
			countDependencies = stoi(line);
		} catch (...){
			clear();
			throw new LeafError(Error::PACKAGE_STOI, line);
		}
		
		if (!getline(in, line))
			throw new LeafError(Error::PACKAGE_UNEXPECTED_EOF, "file count");
		
		try{
			countFiles = stoi(line);
		} catch (...){
			clear();
			throw new LeafError(Error::PACKAGE_STOI, line);
		}
	}

	{	//Parse dependencies
		for (int i = 0; i < countDependencies; i++){
			if (!getline(in, line))
				throw new LeafError(Error::PACKAGE_UNEXPECTED_EOF, "dependency " + std::to_string(i+1) + "/" + std::to_string(countDependencies));

			_dependencies.push_back(line);
		}
	}

	{	//Parse files
		for (int i = 0; i < countFiles; i++){
			if (!getline(in, line))
				throw new LeafError(Error::PACKAGE_UNEXPECTED_EOF, "file " + std::to_string(i) + "/" + std::to_string(countFiles));

			_provided_files.push_back(line);
		}
	}
}
