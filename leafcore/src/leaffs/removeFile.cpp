/**
 * @file		
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		The implementation of 
 * @copyright	Copyright (c) 2022
 */
#include "log.h"
#include "error.h"
#include "leafdebug.h"
#include "leafconfig.h"

#include "leaffs.h"

#include <filesystem>
namespace fs = std::filesystem;

void removeFile(std::string path, bool errorOnNotExisting){
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
			throw new LeafError(Error::LEAFFS_FILE_NOT_EXISTING, path);
	}

	if (ec)
		throw new LeafError(Error::REMOVEFILE, path);
}
