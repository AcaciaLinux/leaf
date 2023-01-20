/**
 * @file		leafcore/createCacheDirs.cpp
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		The implementation of createCacheDirs()
 * @copyright	Copyright (c) 2022
 */
#include "log.h"
#include "leafdebug.h"
#include "leafconfig.h"

#include "leafcore.h"
#include "leaffs.h"

#include <filesystem>

void Leafcore::createCacheDirs(){
	FUN();
	LEAF_DEBUG_EX("Leafcore::createCacheDirs()");
	checkDirectories();

	LeafFS::create_directories(_config.downloadDir());
	LeafFS::create_directories(_config.packagesDir());
}
