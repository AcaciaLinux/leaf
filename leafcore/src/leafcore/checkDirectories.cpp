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
		if (!std::filesystem::exists(_config.configDir(), ec)){
			if (ec){
				throw new LeafError(Error::FS_ERROR, "Configuration directory " + _config.configDir(), ec);
			}

			if (askUserOK("Configuration directory " + _config.configDir() + " does not exist, create it?", true)){
				LOGU("Creating leaf config directory");
				std::filesystem::create_directories(_config.configDir(), ec);
				if (ec){
					throw new LeafError(Error::CREATEDIR, "Configuration directory " + _config.configDir(), ec);
				}
			}
			else
			{
				throw new LeafError(Error::USER_DISAGREE, "Creating config directory " + _config.configDir());
			}
		}

		if (!std::filesystem::is_directory(_config.configDir())){
			throw new LeafError(Error::NOTDIR, "Configuration directory " + _config.configDir());
		}
	}

	{//Check the cache directory
		std::error_code ec;
		LOGD("Checking cache directory...");
		if (!std::filesystem::exists(_config.cacheDir(), ec)){
			if (ec){
				throw new LeafError(Error::FS_ERROR, "Cache directory " + _config.cacheDir(), ec);
			}

			if (askUserOK("Cache directory " + _config.cacheDir() + " does not exist, create it?", true)){
				LOGU("Creating leaf cache directory");
				std::filesystem::create_directories(_config.cacheDir(), ec);
				if (ec){
					throw new LeafError(Error::CREATEDIR, "Cache directory " + _config.cacheDir(), ec);
				}
			}
			else
			{
				throw new LeafError(Error::USER_DISAGREE, "Creating cache directory " + _config.cacheDir());
			}
		}

		if (!std::filesystem::is_directory(_config.cacheDir())){
			throw new LeafError(Error::NOTDIR, "Cache directory " + _config.cacheDir());
		}
	}
}
