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

//TODO: Tests

void Leafcore::a_installLocal(std::deque<std::string> packages){
	FUN();
	LEAF_DEBUG_EX("Leafcore::a_installLocal()");

	checkDirectories();

	//Check if there even are some packages to process
	if (packages.size() == 0)
		throw new LeafError(Error::NOPACKAGE);

	//Create the list of packages to install
	std::deque<Package*> install_packages;
	for (std::string pkg : packages){
		install_packages.push_back(Package::CreateFromLocal(pkg));
	}

	{//Inform the user about the action
		std::string msg = "Installing following local packages:";
		for (Package* p : install_packages)
			msg += " " + p->getFullName();
		LOGU(msg);
	}

}
