/**
 * @file        leafcore/download_packages.cpp
 * @author      Max Kofler (kofler.max.dev@gmail.com)
 * @brief       The implementation of the function Leafcore::download_packages()
 * @copyright   Copyright (c) 2022 Max Kofler and the AcaciaLinux developers
 */
#include "log.h"
#include "error.h"
#include "leafdebug.h"

#include "leafcore.h"

//TODO: Tests

void Leafcore::download_packages(const std::deque<Package*>& download_packages){
	FUN();
	LEAF_DEBUG_EX("Leafcore::download_packages()");

	{//Check for the redownload config and delete cached downloads if neccesary

		LOGI("Checking for redownloads...");
		switch(_config.redownload){
			//If the redownload of the specified files is wanted
			case CONFIG_REDOWNLOAD_SPECIFIED:

			//If the redownload of all packages and dependencies is wanted
			case CONFIG_REDOWNLOAD_ALL:{
				LOGU("Removing download cache of all packages and dependencies...");

				for (Package* package : download_packages)
					package->removeDownloadCache();

				break;
			}

			//Skip if there is nothing to remove
			default:
				break;
		}

		//Create the progress object
		Log::conf_Progress conf;
		conf.maxSteps = 50;
		conf.prefix = "Fetching packages...";
		auto progress = hlog->createProgress(conf);

		//Now go over every package and download it
		size_t size_packages = download_packages.size();
		for (size_t cur_pkg = 0; cur_pkg < size_packages; cur_pkg++){
			Package* package = download_packages[cur_pkg];

			progress->update(size_packages-1, cur_pkg-1, std::to_string(cur_pkg+1) + " / " + std::to_string(size_packages));

			package->fetch();
		}

		//Update the progressbar to make it complete
		progress->update(size_packages-1, size_packages-1, std::to_string(size_packages) + " / " + std::to_string(size_packages));
	}
}
