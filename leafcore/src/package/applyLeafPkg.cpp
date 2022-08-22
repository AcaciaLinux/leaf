/**
 * @file		
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		The implementation of 
 * @copyright	Copyright (c) 2022
 */
#include "log.h"
#include "error.h"
#include "leafdebug.h"

#include "package.h"

//TODO: Tests

void Package::applyLeafPkg(leafpkg_t lfpkg){
	FUN();
	LEAF_DEBUG_EX("Package::applyLeafPkg()");

	_name = lfpkg.name;
	_versionString = lfpkg.version;
	_realVersion = lfpkg.real_version;
	_description = lfpkg.description;
	_dependencies = lfpkg.dependencies;

}
