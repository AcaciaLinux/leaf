/**
 * @file		leafcore/a_installLocal.cpp
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		The implementation of Leafcore::a_installLocal()
 * @copyright	Copyright (c) 2022
 */
#include "log.h"
#include "leafdebug.h"
#include "leafconfig.h"

#include "leafcore.h"

#include <filesystem>
#include <fstream>

//TODO: Tests

void Leafcore::a_installLocal(std::deque<std::string> packages){
	FUN();
	LEAF_DEBUG_EX("Leafcore::a_installLocal()");

	checkDirectories();

	//Check if there even are some packages to process
	if (packages.size() == 0)
		throw new LeafError(Error::NOPACKAGE);

	LeafDB dummyDB(this);

	//Create the list of packages to install
	std::deque<Package*> install_packages;
	for (std::string pkg : packages){
		Package* newPack = Package::CreateFromLocal(pkg);
		newPack->setDB(&dummyDB);
		install_packages.push_back(newPack);
	}

	{	//Go through every package and see if it is already installed
		LOGI("Checking for already installed packages...");
		size_t len_packages = install_packages.size();
		for (size_t i = 0; i < len_packages; i++){

			//Get the package to check
			Package* p_check = install_packages.at(i);
			LOGD("Checking if " + p_check->getName() + " is already installed...");

			//Find it in the installed db
			Package* p = _installedDB->getPackage(p_check->getName(), false);
			
			//If the package is already installed, skip its installation
			if (p != nullptr){
				LOGI(p_check->getName() + " is already installed: YES");
				LOGU("Package " + p_check->getFullName() + " is already installed!");
				install_packages.erase(install_packages.begin() + i);
				i--;
				len_packages--;
			} else {
				LOGI(p_check->getName() + " is already installed: NO");
			}
		}
	}

	//If there is nothing to do, exit this function
	if (install_packages.size() == 0){
		LOGU("There is nothing to do");
		return;
	}

	{//Inform the user about the action
		std::string msg = "Installing following local packages:";
		for (Package* p : install_packages)
			msg += " " + p->getFullName();
		LOGU(msg);
	}

	{//Ask the user for permission
		if (!askUserOK("Do you want to continue?", true)){
			std::string msg = "Installing packages:";
			for (Package* pkg : install_packages)
				msg += " " + pkg->getFullName();
			throw new LeafError(Error::USER_DISAGREE, msg);
		}
	}

	for (Package* package : install_packages){
		LOGU("Extracting package " + package->getFullName() + "...");
		package->extract();
	}

	//Apply the leaf.pkg file to every package
	for (Package* package : install_packages){
		std::ifstream pkgFile;
		std::string pkgFilePath = package->getExtractedDir() + "/leaf.pkg";
		LOGU("Applying leaf.pkg file for " + package->getFullName() + "...");

		pkgFile.open(pkgFilePath, std::ios::in);
		if (!pkgFile.is_open())
			throw new LeafError(Error::OPENFILER, "leaf.pkg of " + package->getFullName() + " at " + pkgFilePath);

		leafpkg_t lfpkg = LeafPkg::parse(pkgFile);
		package->applyLeafPkg(lfpkg);
		LOGU("New package: " + package->toString());
	}

	for (Package* package : install_packages){
		LOGU("Deploying package " + package->getFullName() + "...");
		
		package->deploy();
	}

	if (!_config.noClean){
		LOGU("Cleaning up package caches...");
		for (Package* package : install_packages){
			package->clearCache();
			delete package;
		}
	}
}
