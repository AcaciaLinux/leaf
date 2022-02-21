/**
 * @file		package/fetch.cpp
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		The code for fetching a package from  the server
 * @copyright 	Copyright (c) 2022
 */

#include "log.h"
#include "fail.h"
#include "package.h"
#include "leafconfig.h"

bool Package::resolveDependentPackages(){
	FUN();
	_error.clear();
	std::string _ep = "Failed to resolve dependent packages of " + getFullName() + ": ";

	//Check if the database is ok
	if (_db == nullptr){
		_error = _ep + "Database is not accessible (nullptr)";
		return FAIL(_error);
	}

	_error = _ep + "Feature not implemented";
	return FAIL(_error);

	return true;
}
