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