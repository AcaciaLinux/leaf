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

bool LeafDB::renamePackage(std::string oldName, std::string newName){
	FUN();
	LEAF_DEBUG_EX("LeafDB::renamePackage()");

	LOGD("Renaming package " + oldName + " to " + newName);

	if (_packages.count(oldName) == 0){
		LOGE("Could not find package " + oldName + " for renaming");
		return false;
	}

	Package* buf = _packages[oldName];
	_packages.erase(oldName);
	_packages[newName] = buf;

	return true;
}
