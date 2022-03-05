/**
 * @file		leafcore/createConfigDirs.cpp
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		The implementation of Leafcore::createConfigDirs()
 * @copyright	Copyright (c) 2022
 */
#include "log.h"
#include "error.h"
#include "leafdebug.h"
#include "leafconfig.h"

#include "leafcore.h"

#include <filesystem>

bool Leafcore::createConfigDirs(){
	FUN();
	LEAF_DEBUG("Leafcore::createConfigDirs()");

	if (!checkDirectories())
		return false;

	{//Check the installed directory
		std::error_code ec;
		std::string eMsg = "Installed directory " + lConfig.installedDir();

		//Check if the directory exists
		bool exists = std::filesystem::exists(lConfig.installedDir(), ec);

		if (ec)
			throw new LeafError(Error::FS_ERROR, eMsg, ec);

		//Check if the filesystem entry exists
		if (!exists){
			std::filesystem::create_directories(lConfig.installedDir(), ec);

			if (ec)
				throw new LeafError(Error::CREATEDIR, eMsg, ec);
		}
		else	//Else check if the filesystem entry is a directory
		{
			bool isDirectory = std::filesystem::is_directory(lConfig.installedDir(), ec);

			if (ec)
				throw new LeafError(Error::FS_ERROR, eMsg, ec);

			if (!isDirectory)
				throw new LeafError(Error::NOTDIR, eMsg, ec);
		}
	}

	return true;
}
