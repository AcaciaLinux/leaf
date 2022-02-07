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