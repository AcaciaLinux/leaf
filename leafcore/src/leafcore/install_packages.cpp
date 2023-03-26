/**
 * @file        leafcore/install_packages.cpp
 * @author      Max Kofler (kofler.max.dev@gmail.com)
 * @brief       The implementation of the function Leafcore::install_packages()
 * @copyright   Copyright (c) 2022 Max Kofler and the AcaciaLinux developers
 */
#include "log.h"
#include "error.h"
#include "leafdebug.h"

#include "leafcore.h"

//TODO: Tests

void Leafcore::install_packages(const std::deque<Package*>& packages){
	FUN();
	LEAF_DEBUG_EX("Leafcore::install_packages()");

	//Create the progress object
	Log::conf_Progress conf;
	conf.maxSteps = 50;
	conf.prefix = "Installing packages...";
	auto progress = hlog->createProgress(conf);

	//Go over every package, extract and then deploy it
	size_t size_packages = packages.size();
	for (size_t cur_pkg = 0; cur_pkg < packages.size(); cur_pkg++){
		Package* package = packages[cur_pkg];

		LOGU("Installing package " + package->getFullName() + "...");
		progress->update(packages.size()-1, cur_pkg-1, std::to_string(cur_pkg+1) + " / " + std::to_string(packages.size()));

		package->extract();

		package->deploy();
	}

	//Update the progressbar to make it complete
	progress->update(size_packages-1, size_packages-1, std::to_string(size_packages) + " / " + std::to_string(size_packages));
}
