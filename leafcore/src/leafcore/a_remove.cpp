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
			Package* package = _installedDB->getPackage(packageName);

			if (package == nullptr){
				_error = _ep + "Package " + packageName + " does not seem to be installed";
				return FAIL(_error);
			}

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

	if (!askUserOK("Do you want to continue?", true)){
		_error = "Aborted by user";
		return FAIL(_error);
	}

	for (Package* p : remove_packages){
		LOGU("Removing package " + p->getFullName() + "...");

		if (!p->removeFromRoot()){
			_error = _ep + p->getError();
			return FAIL(_error);
		}
	}

	_error = _ep + "Feature not implemented";
	return FAIL(_error);

	return true;
}