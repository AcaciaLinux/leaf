/**
 * @file		leafcore/a_upgrade.cpp
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		The implementation of Leafcore::a_upgrade()
 * @copyright	Copyright (c) 2022
 */
#include "log.h"
#include "leafdebug.h"
#include "leafconfig.h"

#define FRIEND_LEAFDB
#include "leafcore.h"

#include "util.h"

#include <filesystem>
#include <map>

void Leafcore::a_upgrade(std::deque<std::string> packages){
	FUN();
	LEAF_DEBUG_EX("Leafcore::a_upgrade()");

	checkDirectories();

	//Check if the package list is loaded
	if (!_loadedPkgList)
		throw new LeafError(Error::PKGLIST_NOTLOADED);

	if (packages.size() == 0){
		LOGU("Upgrading system...");
		for (const auto& i : _installedDB->_packages){
			packages.push_back(i.first);
		}
	}

	size_t duplicates = LeafUtil::removeDuplicates(packages);
	if (duplicates != 0)
		LOGI("[Leafcore][a_upgrade] " + std::to_string(duplicates) + " packages remaining after removeDuplicates()");
	
	std::deque<Package*> installedPkg;
	{	//Resolve package names
		Package* curPkg;
		for (std::string pkgName : packages){
			curPkg = _installedDB->getPackage(pkgName, false);

			if (curPkg == nullptr)
				LOGU("Package " + pkgName + " does not seem to be installed, can not be upgraded");
			else {
				LOGD("[Leafcore][a_upgrade] Resolved installed package " + curPkg->getName());
				installedPkg.push_back(curPkg);
			}
		}
	}

	std::map<Package*, Package*> availablePkgs;
	{	//Resolve remote packages (those in the package list)
		Package* curPkg;
		for (Package* localPkg : installedPkg){
			curPkg = _packageListDB->getPackage(localPkg->getName(), true);

			LOGD("[Leafcore][a_upgrade] Resolved remote package " + curPkg->getName());
			availablePkgs[localPkg] = curPkg;
		}
	}

	std::map<Package*, Package*> upgradePkgs;
	{	//Compare realversions
		for (const auto &i : availablePkgs){
			bool changePkg = i.first->getRealVersion() != i.second->getRealVersion();

			LOGI(	"[Leafcore][a_upgrade] Checking change of " + i.first->getName() + 
					": " + i.first->getVersion() + " (" + std::to_string(i.first->getRealVersion()) + ") " + 
					"-> " + i.second->getVersion() + " (" + std::to_string(i.second->getRealVersion()) + "): " + 
					(changePkg?"Yes, staging for change...":"No"));

			if (changePkg){
				upgradePkgs[i.first] = i.second;
			}
		}
	}
}
