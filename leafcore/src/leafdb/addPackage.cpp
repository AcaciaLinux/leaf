/**
 * @file		
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		The implementation of 
 * @copyright	Copyright (c) 2022
 */
#include "log.h"
#include "error.h"
#include "leafdebug.h"
#include "leafconfig.h"

#include "leafdb.h"

bool LeafDB::addPackage(Package* newPackage){
	FUN();
	LEAF_DEBUG_EX("LeafDB::addPackage()");

	//Check if the package can be used
	if (newPackage == nullptr)
		throw new LeafError(Error::NULLPARAMETER, "LeafDB::addPackage(<newPackage>)");

	//Check for existing package
	for (auto pkg : _packages){
		if (pkg.second->getName() == newPackage->getName()){
			LOGE("LeafDB: Package " + pkg.second->getName() + " already in database");
			return false;
		}
	}

	//Set the database
	newPackage->setDB(this);

	_packages[newPackage->getName()] = newPackage;
	
	return true;
}
