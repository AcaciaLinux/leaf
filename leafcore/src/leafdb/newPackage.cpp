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

Package* LeafDB::newPackage(std::string name, std::string version){
	FUN();
	LEAF_DEBUG_EX("LeafDB::newPackage()");

	Package* pkg = new Package(name, version);

	pkg->setDB(this);
	_packages[name] = pkg;
	
	return pkg;
}
