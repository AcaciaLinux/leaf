/**
 * @file		package/copyToRoot.cpp
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		The code for copying the extracted package to its destination
 * @copyright 	Copyright (c) 2022
 */

#include "log.h"
#include "leafdebug.h"
#include "package.h"
#include "leafconfig.h"

#include "leaffs.h"

#include <filesystem>

void Package::copyToRoot(bool forceOverwrite){
	FUN();
	LEAF_DEBUG_EX("Package::copyToRoot()");

	if (lConfig.forceOverwrite)
		forceOverwrite = true;

	LOGI("Copying package " + getFullName() + " to root " + lConfig.rootDir);

	namespace fs = std::filesystem;

	if (!fs::exists(lConfig.rootDir))
		throw new LeafError(Error::NOROOT, lConfig.rootDir);

	if (!fs::exists(getExtractedDir()))
		throw new LeafError(Error::PKG_NOTEXTRACTED, getFullName());

	indexExtracted();

	//If leaf should not overwrite existing files, check for them
	if (!forceOverwrite){
		//Got through every entry
		for (std::string file : _provided_files){
			
			//Directories can be overwritten
			if (!std::filesystem::is_directory(lConfig.rootDir + file)){

				//If the file exists, error out
				if (std::filesystem::exists(lConfig.rootDir + file))
					throw new LeafError(Error::PACKAGE_FILE_EXISTS, lConfig.rootDir + file);
			}
		}
	}

	//Finally, copy the package
	std::error_code ec;

	fs::copy_options options = fs::copy_options::copy_symlinks;

	std::string dataDir = getExtractedDir() + "data/";
	std::string destDir = lConfig.rootDir;

	LOGI("Creating destination directories...");

	//First, create the directories for the files
	for (std::string dir : _provided_directories){
		LOGF("Creating directory " + destDir + dir);

		std::filesystem::create_directories(destDir + dir, ec);

		if (ec)
			throw new LeafError(Error::CREATEDIR, destDir + dir, ec);
	}

	LOGI("Copying files...");

	//Now copy all the files
	for (std::string file : _provided_files){

		//If leaf should overwrite the files, delete the old files
		if (forceOverwrite){
			removeFile(destDir + file, false);
		}

		LOGF("Copying " + dataDir + file + " -> " + destDir + file);
		fs::copy(dataDir + file, destDir + file, options, ec);
		
		if (ec)
			throw new LeafError(Error::COPYFILE, dataDir + file, ec);
	}
}
