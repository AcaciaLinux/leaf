/**
 * @file		package/removeFromRoot.cpp
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		The code for fetching a package from  the server
 * @copyright 	Copyright (c) 2022
 */

#include "log.h"
#include "leafdebug.h"
#include "package.h"
#include "leafconfig.h"

#include <fstream>
#include <filesystem>

void Package::removeFromRoot(){
	FUN();
	LEAF_DEBUG_EX("Package::removeFromRoot()");

	//Check if the database is ok
	if (_db == nullptr)
		throw new LeafError(Error::NODB);

	std::string rootDir = _db->getCore()->getConfig().rootDir;

	std::error_code ec;
	for (std::string entry : _provided_files){
		if (std::filesystem::is_directory(rootDir + entry)){
			if (std::filesystem::is_empty(rootDir + entry)){
				LOGF("Removing empty directory " + rootDir + entry);
			} else {
				LOGF("Skipping non empty directory " + rootDir + entry);
				continue;
			}
		}

		LOGF("Removing entry " + rootDir + entry);
		std::filesystem::remove_all(rootDir + entry, ec);

		if (ec)
			throw new LeafError(Error::REMOVE, getFullName() + ": " + rootDir + entry);
	}

	//Finally remove the leafinstalled file
	std::filesystem::remove(getInstalledFilePath(), ec);

	if (ec)
		throw new LeafError(Error::REMOVEFILE, "Leafinstalled file of " + getFullName() + " at " + getInstalledFilePath());
}
