/**
 * @file		leafdb/resolveDependencies.cpp
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		The dependency resolving part of LeafDB
 * @copyright 	Copyright (c) 2022
 */

#include "log.h"
#include "error.h"
#include "leafdebug.h"
#include "leafconfig.h"

#include "leafdb.h"

bool LeafDB::resolveDependencies(std::deque<Package*>* all_dependencies, Package* package){
	FUN();
	LOGI("Resolving dependencies for " + package->getName() + "...");

	{//If this packages is already a dependency, do not resolve dependencies again
		for (Package* pkg : *all_dependencies){
			if (pkg->getName() == package->getName()){
				return true;
			}
		}
	}

	//Add the package temporarily, pull it back afterwards
	all_dependencies->push_back(package);
	size_t pkg_temp_pos = all_dependencies->size()-1;

	for (std::string depName : package->getDependencies()){
		//Get the package from the name
		Package* dependency = getPackage(depName, false);

		//Check if the dependency was found
		if (dependency == nullptr)
			throw new LeafError(Error::LEAFDB_PKG_DEP_NOTFOUND, depName);

		bool alreadyAdded = false;
		for (size_t i = 0; i < all_dependencies->size(); i++){
			if (all_dependencies->at(i)->getName() == depName){
				alreadyAdded = true;
				LOGD("Pulling back dependency " + depName);
				all_dependencies->erase(all_dependencies->begin() + i);
				break;
			}
		}

		//If the dependency has already been added, skip the dependency search
		if (!alreadyAdded){
			//Scan for dependencies of the dependency
			if (!resolveDependencies(all_dependencies, dependency))
				return false;
		} else {
			LOGD("Skipping scanning for dependencies for pulled back package " + depName);
		}
	}

	//Remove the temporarily added package and add the package finally
	all_dependencies->erase(all_dependencies->begin() + pkg_temp_pos);
	all_dependencies->push_back(package);

	return true;
}

std::deque<Package*> LeafDB::resolveDependencies(Package* package){
	FUN();
	_error.clear();
	std::deque<Package*> dependencies;

	if (!resolveDependencies(&dependencies, package)){
		_error = "Failed to find dependency package " + _error;
		dependencies.clear();
	}

	return dependencies;
}
