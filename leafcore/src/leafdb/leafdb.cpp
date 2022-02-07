#include "log.h"
#include "leafdb.h"

LeafDB::LeafDB(){
	FUN();
}

LeafDB::~LeafDB(){
	FUN();

	for (auto entry : _packages){
		LOGD("Removing package " + entry.first);
		delete entry.second;
	}

	_packages.clear();
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

Package* LeafDB::newPackage(std::string name){
	LOGD("Adding new package " + name);

	Package* pkg = new Package(name);
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

	for (std::string depString : package->getDependencies()){
		//Resolve the dependency string
		Package* dep = getPackage(depString);

		//Check if the dependency was not found
		if (dep == nullptr){
			_error = depString;
			LOGE("Failed to resolve dependency package " + _error);
			return false;
		}

		//Check if the package has not been added already (circular dependency)
		for (Package* pkg : *dependencies){
			if (pkg == dep){
				LOGI("Circular dependency: " + dep->getName());
				continue;
			}
		}

		//Add the dependency and scan it for its dependencies
		dependencies->push_back(dep);
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

std::string LeafDB::getError(){
	return _error;
}