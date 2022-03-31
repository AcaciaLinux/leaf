/**
 * @file		pkglistparser/applyToDB.cpp
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		The implementation of PackageListParser::applyToDB()
 * @copyright 	Copyright (c) 2022
 */

#include "log.h"
#include "error.h"
#include "pkglistparser.h"

void PackageListParser::applyToDB(LeafDB& db){
	FUN();

	bool error = false;
	std::string errString = "";

	for (Package* package : _packages){
		if (!db.addPackage(package)){
			LOGE("Failed to add package " + package->getName() + ", already existing");
			error = true;
			errString += "[" + package->getName() + "] ";
			
			//If the apply fails, clean the memory
			delete package;
		}
	}

	//Deploy a readable error message
	if (error)
		throw new LeafError(Error::PKGPRS_APPLY_DB, errString);

	//Clear the packages vector, the DB now owns the packages and manages them
	_packages.clear();
}
