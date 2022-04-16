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

#include "leaffs.h"

#include <filesystem>
namespace fs = std::filesystem;

void LeafFS::check(){
	FUN();
	LEAF_DEBUG_EX("LeafFS::check()");

	if (_curDir.empty())
		throw new LeafError(Error::LEAFFS_DIR_STR_EMPTY, _curDir);

	if (!fs::exists(_curDir))
		throw new LeafError(Error::LEAFFS_DIR_NOT_EXISTING, _curDir);

	if (!fs::is_directory(_curDir))
		throw new LeafError(Error::LEAFFS_DIR_NOT_DIR, _curDir);
}
