/**
 * @file		leafcore/createCacheDirs.cpp
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		The implementation of createCacheDirs()
 * @copyright	Copyright (c) 2022
 */
#include "log.h"
#include "error.h"
#include "leafdebug.h"
#include "leafconfig.h"

#include "leafcore.h"

#include <filesystem>

bool Leafcore::createCacheDirs(){
	FUN();
	LEAF_DEBUG("Leafcore::createCacheDirs()");
	if (!checkDirectories())
		return false;

	{//Check the download directory
		std::error_code ec;
		std::string eMsg = "Download directory " + lConfig.downloadDir();
		
		//Check if the download directory exists
		bool exists = std::filesystem::exists(lConfig.downloadDir(), ec);

		if (ec)
			throw new LeafError(Error::FS_ERROR, eMsg, ec);

		//If the directory does not exists, create it
		if (!exists){
			std::filesystem::create_directories(lConfig.downloadDir(), ec);
			
			if (ec)
				throw new LeafError(Error::CREATEDIR, eMsg, ec);
		}
		else	//Else check if the existing entry is a directory
		{
			bool isDirectory = std::filesystem::is_directory(lConfig.downloadDir(), ec);

			if (ec)
				throw new LeafError(Error::FS_ERROR, eMsg, ec);

			if (!isDirectory)
				throw new LeafError(Error::NOTDIR, eMsg, ec);
		}
	}
	
	{//Check the packages directory
		std::error_code ec;
		std::string eMsg = "Packages directory " + lConfig.packagesDir();

		//Check if the packages directory exists
		bool exists = std::filesystem::exists(lConfig.packagesDir(), ec);

		if (ec)
			throw new LeafError(Error::FS_ERROR, eMsg, ec);

		//If the directory does not exists, create it
		if (!exists){
			std::filesystem::create_directories(lConfig.packagesDir(), ec);
			
			if (ec)
				throw new LeafError(Error::CREATEDIR, eMsg, ec);
		}
		else	//Else check if the existing entry is a directory
		{
			bool isDirectory = std::filesystem::is_directory(lConfig.packagesDir(), ec);

			if (ec)
				throw new LeafError(Error::FS_ERROR, eMsg, ec);

			if (!isDirectory)
				throw new LeafError(Error::NOTDIR, eMsg);
		}
	}

	return true;
}
