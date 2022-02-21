/**
 * @file		leafdb/leafdb.cpp
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		The core implementation of LeafDB
 * @copyright 	Copyright (c) 2022
 */

#include "log.h"
#include "fail.h"
#include "leafdb.h"

LeafDB::LeafDB(Leafcore* core){
	FUN();

	_core = core;
}

LeafDB::~LeafDB(){
	FUN();

	this->clear();
}

bool LeafDB::addPackage(Package* newPackage){
	FUN();

	//TODO check newPackage for nullptr

	//Check for existing package
	for (auto pkg : _packages){
		if (pkg.second->getName() == newPackage->getName()){
			_error = "LeafDB: Package " + pkg.second->getName() + " already in database";
			return FAIL(_error);
		}
	}

	//Set the database
	newPackage->setDB(this);

	_packages[newPackage->getName()] = newPackage;
	
	return true;
}

Package* LeafDB::getPackage(std::string name){
	FUN();

	LOGD("Searching for package with name " + name);

	if (_packages.count(name) == 0)
		return nullptr;

	return _packages[name];
}

Package* LeafDB::newPackage(std::string name, std::string version){
	Package* pkg = new Package(name, version);
	pkg->setDB(this);
	_packages[name] = pkg;
	return pkg;
}

bool LeafDB::renamePackage(std::string oldName, std::string newName){
	FUN();

	LOGD("Renaming package " + oldName + " to " + newName);

	if (_packages.count(oldName) == 0)
		return false;

	Package* buf = _packages[oldName];
	_packages.erase(oldName);
	_packages[newName] = buf;

	return true;
}

std::deque<Package*> LeafDB::findFileProviders(std::string filepath){
	FUN();
	std::deque<Package*> providers;

	for(auto package : _packages){
		if (package.second == nullptr)
			continue;

		if (package.second->checkFileProvided(filepath))
			providers.push_back(package.second);
	}

	return providers;
}

void LeafDB::clear(){
	FUN();

	for (auto entry : _packages){
		delete entry.second;
	}

	_packages.clear();
}