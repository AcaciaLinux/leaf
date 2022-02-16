/**
 * @file		leafdb/leafdb.cpp
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		The core implementation of LeafDB
 * @copyright 	Copyright (c) 2022
 */

#include "log.h"
#include "leafdb.h"

LeafDB::LeafDB(){
	FUN();
}

LeafDB::~LeafDB(){
	FUN();

	this->clear();
}

bool LeafDB::addPackage(Package* newPackage){
	FUN();

	//Check for existing package
	for (auto pkg : _packages){
		if (pkg.second->getName() == newPackage->getName())
			return false;
	}

	_packages[newPackage->getName()] = newPackage;
	
	return true;
}

Package* LeafDB::getPackage(std::string name){
	FUN();

	if (_packages.count(name) == 0)
		return nullptr;

	return _packages[name];
}

Package* LeafDB::newPackage(std::string name, std::string version){
	LOGD("Adding new package " + name);

	Package* pkg = new Package(name, version);
	_packages[name] = pkg;
	return pkg;
}

std::vector<Package*> LeafDB::findFileProviders(std::string filepath){
	FUN();
	std::vector<Package*> providers;

	for(auto package : _packages){
		if (package.second == nullptr)
			continue;

		if (package.second->checkFileProvided(filepath))
			providers.push_back(package.second);
	}

	return providers;
}

bool LeafDB::resolveDependencies(std::vector<Package*>* dependencies, Package* package){
	FUN();
	LOGI("Resolving dependencies for " + package->getName() + "...");

	for (std::string depString : package->getDependencies()){
		//Resolve the dependency string
		Package* dep = getPackage(depString);

		//Check if the dependency was not found
		if (dep == nullptr){
			_error = depString;
			LOGE("Failed to resolve dependency package " + _error);
			return false;
		}

		bool circular_dependency = false;
		//Check if the package has not been added already (circular dependency)
		for (Package* pkg : *dependencies){
			if (pkg == dep){
				LOGI("Circular dependency: " + dep->getName());
				circular_dependency = true;
			}
		}

		if (circular_dependency)
			continue;

		//Add the dependency and scan it for its dependencies

		//If the package has a download URL, add it
		if (!dep->getFetchURL().empty())
			dependencies->push_back(dep);
		
		else
			//If there are dependenceis, treat the package as collection
			if (!dep->getDependencies().empty())
				LOGI("Treating package " + dep->getName() + " as collection");
			//Else error out
			else{
				_error = "Package " + package->getName() + " has no fetch URL and is no collection";
				LOGE("Failed resolve dependencies: " + _error);
				return false;
			}

		if (!resolveDependencies(dependencies, dep))
			return false;
	}

	return true;
}

std::vector<Package*> LeafDB::resolveDependencies(Package* package){
	FUN();
	_error.clear();
	std::vector<Package*> dependencies;

	if (!resolveDependencies(&dependencies, package)){
		_error = "Failed to find dependency package " + _error;
		dependencies.clear();
	}

	return dependencies;
}

void LeafDB::clear(){
	FUN();

	for (auto entry : _packages){
		delete entry.second;
	}

	_packages.clear();
}
