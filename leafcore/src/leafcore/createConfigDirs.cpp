/**
 * @file		leafcore/createConfigDirs.cpp
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		The implementation of Leafcore::createConfigDirs()
 * @copyright	Copyright (c) 2022
 */
#include "log.h"
#include "leafdebug.h"
#include "leafconfig.h"

#include "leafcore.h"
#include "leaffs.h"

#include <filesystem>

void Leafcore::createConfigDirs(){
	FUN();
	LEAF_DEBUG_EX("Leafcore::createConfigDirs()");

	checkDirectories();

	LeafFS::create_directories(_config.installedDir());
	LeafFS::create_directories(_config.hooksDir());
}
