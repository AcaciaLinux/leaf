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
#include "globals.h"

#include "leaffs.h"

#include <filesystem>

void Package::copyToRoot(bool forceOverwrite){
	FUN();
	LEAF_DEBUG_EX("Package::copyToRoot()");

	//Check if the database is ok
	if (_db == nullptr)
		throw new LeafError(Error::NODB);

	std::string rootDir = _db->getCore()->getConfig().rootDir;

	LOGI("Copying package " + getFullName() + " to root " + rootDir);

	namespace fs = std::filesystem;

	if (!fs::exists(rootDir))
		throw new LeafError(Error::NOROOT, rootDir);

	if (!fs::exists(getExtractedDir()))
		throw new LeafError(Error::PKG_NOTEXTRACTED, getFullName());

	indexExtracted();

	//If leaf should not overwrite existing files, check for them
	if (!forceOverwrite){
		//Got through every entry
		for (std::string file : _provided_files){

			//If the entry exists and is not a directory, error out
			if (LeafFS::exists(rootDir + file) && !LeafFS::is(rootDir + file, LEAFFS_DIR))
				throw new LeafError(Error::PACKAGE_FILE_EXISTS, rootDir + file);
		}
	}

	//Finally, copy the package
	std::error_code ec;

	fs::copy_options options = fs::copy_options::copy_symlinks;

	std::string dataDir = getExtractedDir() + "data/";
	std::string destDir = rootDir;

	LOGI("Creating destination directories...");

	//First, create the directories for the files
	for (std::string dir : _provided_directories){
		LOGF("Creating directory " + destDir + dir);

		if (LeafFS::exists(destDir + dir) && LeafFS::is(destDir + dir, LEAFFS_SYMLINK)){
			LOGI("[Package::copyToRoot] Skipping creation of directory that is a symlink");
			continue;
		}

		LeafFS::create_directories(destDir + dir);
	}

	LOGI("Copying files...");

	std::deque<std::string> copied_files;
	//Now copy all the files
	for (std::string file : _provided_files){
		if (!proceed){
			LOGUW("Copy for package " + getFullName() + " aborted, rolling back...");
			_provided_files = copied_files;

			try {
				removeFromRoot();
			} catch (LeafError* e) {
				e->prepend("When rolling back copy for package " + getFullName() + ": ");
			}

			throw new LeafError(Error::ABORT);
		}

		//If leaf should overwrite the files, delete the old files
		if (forceOverwrite){
			removeFile(destDir + file, false);
		}

		LOGF("Copying " + dataDir + file + " -> " + destDir + file);
		fs::copy(dataDir + file, destDir + file, options, ec);
		copied_files.push_back(file);

		if (ec)
			throw new LeafError(Error::COPYFILE, dataDir + file, ec);
	}
}
