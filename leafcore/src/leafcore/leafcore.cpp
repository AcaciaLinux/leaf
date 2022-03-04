/**
 * @file		leafcore/leafcore.cpp
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		The core implementation of Leafcore
 * @copyright 	Copyright (c) 2022
 */

#define FRIEND_PACKAGE

#include "log.h"
#include "fail.h"
#include "error.h"
#include "leafcore.h"
#include "leafconfig.h"

#include "pkglistparser.h"
#include "downloader.h"
#include "leafarchive.h"
#include "leaffs.h"

#include <fstream>
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
		if (!std::filesystem::exists(lConfig.downloadDir())){
			if (!std::filesystem::create_directories(lConfig.downloadDir())){
				_error = "Failed to create download directory " + lConfig.downloadDir();
				return FAIL(_error);
			}
		} else {
			if (!std::filesystem::is_directory(lConfig.downloadDir())){
				_error = "Download directory " + lConfig.downloadDir() + " is not a directory";
				return FAIL(_error);
			}
		}
	}
	
	{//Check the packages directory
		if (!std::filesystem::exists(lConfig.packagesDir())){
			if (!std::filesystem::create_directories(lConfig.packagesDir())){
				_error = "Failed to create packages directory " + lConfig.packagesDir();
				return FAIL(_error);
			}
		} else {
			if (!std::filesystem::is_directory(lConfig.packagesDir())){
				_error = "Packages directory " + lConfig.packagesDir() + " is not a directory";
				return FAIL(_error);
			}
		}
	}

	return true;
}

bool Leafcore::createConfigDirs(){
	FUN();

	if (!checkDirectories())
		return false;

	{//Check the installed directory
		if (!std::filesystem::exists(lConfig.installedDir())){
			if (!std::filesystem::create_directories(lConfig.installedDir())){
				_error = "Failed to create installed directory " + lConfig.installedDir();
				return FAIL(_error);
			}
		} else {
			if (!std::filesystem::is_directory(lConfig.installedDir())){
				_error = "Installed directory " + lConfig.installedDir() + " is not a directory";
				return FAIL(_error);
			}
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
