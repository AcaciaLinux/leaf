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

#include "fail.h"

#include "leafcore.h"

bool Leafcore::a_remove(std::deque<std::string> packages){
	FUN();

	LEAF_DEBUG("Leafcore::a_remove()");

	//TODO: exceptions

	_error.clear();
	std::string _ep = "Could not remove packages: ";
	if (!checkDirectories())
		return false;

	//Parse the installed files
	if (!parseInstalled()){
		_error = _ep + _error;
		return FAIL(_error);
	}

	std::deque<Package*> remove_packages;
	{//Resolve the packages names
		for (std::string packageName : packages){
			//This throws an error if the package was not found
			Package* package = _installedDB->getPackage(packageName, true);

			remove_packages.push_back(package);
		}
	}

	//TODO: Check if a package depends on the removed packages

	//Inform the user
	{
		std::string outString = "Packages to remove:";
		for (Package* p : remove_packages){
			outString += " " + p->getFullName();
		}
		LOGU(outString);
	}

	{//Ask the user for permission
		if (!askUserOK("Do you want to continue?", true)){
			std::string msg = "Removing packages:";
			for (Package* pkg : remove_packages)
				msg += " " + pkg->getFullName();
			throw new LeafError(Error::USER_DISAGREE, msg);
		}
	}

	for (Package* p : remove_packages){
		LOGU("Removing package " + p->getFullName() + "...");

		p->removeFromRoot();
	}

	return true;
}
