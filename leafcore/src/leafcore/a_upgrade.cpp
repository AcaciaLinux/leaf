/**
 * @file		leafcore/a_upgrade.cpp
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		The implementation of Leafcore::a_upgrade()
 * @copyright	Copyright (c) 2022
 */
#include "log.h"
#include "leafdebug.h"
#include "leafconfig.h"

#include "leafcore.h"

#include <filesystem>

void Leafcore::a_upgrade(std::deque<std::string> packages){
	FUN();
	LEAF_DEBUG_EX("Leafcore::a_upgrade()");

	checkDirectories();

	//Check if the package list is loaded
	if (!_loadedPkgList)
		throw new LeafError(Error::PKGLIST_NOTLOADED);

}
