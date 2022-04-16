/**
 * @file		leaffs/leaffs.cpp
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		The core implementation of LeafFS
 * @copyright 	Copyright (c) 2022
 */

#include "log.h"
#include "leaffs.h"

#include <filesystem>

namespace fs = std::filesystem;

LeafFS::LeafFS(std::string dir){
	FUN();
	
	_curDir = dir;
}

