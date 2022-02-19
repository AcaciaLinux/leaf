/**
 * @file		leafdb/resolveDependencies.cpp
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		The dependency resolving part of LeafDB
 * @copyright 	Copyright (c) 2022
 */

#include "log.h"
#include "fail.h"
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

	//Add the package temporarily, we pull it back afterwards
	all_dependencies->push_back(package);

	//Go through every dependency of the package
	for (std::string dependencyName : package->getDependencies()){
		//Get the package with the name of the dependency
		Package* dependency = getPackage(dependencyName);

		//If the dependency was not found, error
		if (dependency == nullptr){
			_error = "LeafDB: Could not find dependency " + dependencyName + " for package " + package->getFullName();
			return FAIL(_error);
		}

		bool alreadyAdded = false;

		//Check if the dependency has already been added
		for (size_t i = 0; i < all_dependencies->size(); i++){
			if (all_dependencies->at(i)->getName() == dependencyName){
				alreadyAdded = true;
				LOGD("Pulling back dependency " + dependencyName);
				all_dependencies->erase(all_dependencies->begin() + i);
				break;
			}
		}

		//If the dependency would already be added, we kan skip resolving its dependencies
		if (!alreadyAdded){
			//Scan for dependencies of the dependency
			if (!resolveDependencies(all_dependencies, dependency))
				return false;
		}

		//Add the dependency
		all_dependencies->push_back(dependency);
	}

	//Find the package we first added and pull it back
	for (size_t i = 0; i < all_dependencies->size(); i++){
		if (all_dependencies->at(i) == package){
			all_dependencies->erase(all_dependencies->begin() + i);
		}
	}

	//Now finally add the package
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
