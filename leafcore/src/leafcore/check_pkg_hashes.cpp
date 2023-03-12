/**
 * @file        leafcore/check_pkg_hashes.cpp
 * @author      Max Kofler (kofler.max.dev@gmail.com)
 * @brief       The implementation of the function Leafcore::check_pkg_hashes()
 * @copyright   Copyright (c) 2022 Max Kofler and the AcaciaLinux developers
 */
#include "log.h"
#include "error.h"
#include "leafdebug.h"

#include "leafcore.h"

//TODO: Tests

void Leafcore::check_pkg_hashes(const std::deque<Package*>& packages){
	FUN();
	LEAF_DEBUG_EX("Leafcore::check_pkg_hashes()");

	//Create the progress object
	Log::conf_Progress conf;
	conf.maxSteps = 50;
	conf.prefix = "Checking package integrity...";
	auto progress = hlog->createProgress(conf);

	//Go over every entry and check the hash
	size_t size_packages = packages.size();
	for (size_t cur_pkg = 0; cur_pkg < size_packages; cur_pkg++){
		Package* package = packages[cur_pkg];

		progress->update(size_packages-1, cur_pkg-1, std::to_string(cur_pkg+1) + " / " + std::to_string(size_packages));

		package->checkFetchedHash();
	}

	//Update the progressbar to make it complete
	progress->update(size_packages-1, size_packages-1, std::to_string(size_packages) + " / " + std::to_string(size_packages));
}
