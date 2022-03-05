/**
 * @file		leafcore/leafcore.cpp
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		The core implementation of Leafcore
 * @copyright 	Copyright (c) 2022
 */

#include "log.h"
#include "error.h"
#include "leafcore.h"
#include "leafconfig.h"

#include <filesystem>
#include <iostream>

leaf_config_t lConfig;

Leafcore::Leafcore(){
	FUN();

	_packageListDB = new LeafDB(this);
	_installedDB = new LeafDB(this);
}

bool Leafcore::checkDirectories(){
	FUN();

	LOGD("Checking leaf directories...");

	{//Check the leaf directory
		std::error_code ec;
		LOGD("Checking configuration directory...");
		if (!std::filesystem::exists(lConfig.configDir(), ec)){
			if (ec){
				throw new LeafError(Error::FS_ERROR, "Configuration directory " + lConfig.configDir(), ec);
			}

			if (askUserOK("Configuration directory " + lConfig.configDir() + " does not exist, create it?", true)){
				LOGU("Creating leaf config directory");
				std::filesystem::create_directories(lConfig.configDir(), ec);
				if (ec){
					throw new LeafError(Error::CREATEDIR, "Configuration directory " + lConfig.configDir(), ec);
				}
			}
			else
			{
				throw new LeafError(Error::USER_DISAGREE, "Creating config directory " + lConfig.configDir());
			}
		}

		if (!std::filesystem::is_directory(lConfig.configDir())){
			throw new LeafError(Error::NOTDIR, "Configuration directory " + lConfig.configDir());
		}
	}

	{//Check the cache directory
		std::error_code ec;
		LOGD("Checking cache directory...");
		if (!std::filesystem::exists(lConfig.cacheDir(), ec)){
			if (ec){
				throw new LeafError(Error::FS_ERROR, "Cache directory " + lConfig.cacheDir(), ec);
			}

			if (askUserOK("Cache directory " + lConfig.cacheDir() + " does not exist, create it?", true)){
				LOGU("Creating leaf cache directory");
				std::filesystem::create_directories(lConfig.cacheDir(), ec);
				if (ec){
					throw new LeafError(Error::CREATEDIR, "Cache directory " + lConfig.cacheDir(), ec);
				}
			}
			else
			{
				throw new LeafError(Error::USER_DISAGREE, "Creating cache directory " + lConfig.cacheDir());
			}
		}

		if (!std::filesystem::is_directory(lConfig.cacheDir())){
			throw new LeafError(Error::NOTDIR, "Cache directory " + lConfig.cacheDir());
		}
	}

	return true;
}

bool Leafcore::createCacheDirs(){
	FUN();
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

bool Leafcore::createConfigDirs(){
	FUN();

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

bool Leafcore::askUserOK(std::string question, bool defaultOption){
	FUN();

	if (lConfig.noAsk)
		return true;

	if (defaultOption){
		LOGD("User question: Default action: true");

		std::cout << question << " (Y/n): ";
		std::string answer;
		getline(std::cin, answer);

		LOGD("User question: Answer: \"" + answer + "\"");
		
		if (answer == "y" || answer == "Y" || answer == ""){
			LOGD("User question: User agreed");
			return true;
		} else {
			LOGD("User question: User disagreed");
			return false;
		}
	} 
	else
	{
		std::cout << question << " (y/N): ";
		std::string answer;
		getline(std::cin, answer);
		
		if (answer == "n" || answer == "N" || answer == "")
			return false;
		else 
			return true;
	}
}
