/**
 * @file		leafcore/checkDirectories.cpp
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		The implementation of checkDirectories()
 * @copyright	Copyright (c) 2022
 */
#include "log.h"
#include "leafdebug.h"
#include "leafconfig.h"

#include "leafcore.h"

#include <filesystem>

void Leafcore::checkDirectories(){
	FUN();
	LEAF_DEBUG_EX("Leafcore::checkDirectories()");

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
}
