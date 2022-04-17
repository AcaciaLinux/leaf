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

Package* LeafDB::getPackage(std::string name, bool throwOnNotFound){
	FUN();
	LEAF_DEBUG_EX("LeafDB::getPackage()");

	LOGD("Searching for package with name " + name);

	if (_packages.count(name) == 0){
		if (throwOnNotFound)
			throw new LeafError(Error::LEAFDB_PKG_NOT_FOUND, name);
		else
			return nullptr;
	}

	return _packages[name];
}
