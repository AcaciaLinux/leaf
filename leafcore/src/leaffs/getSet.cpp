/**
 * @file		leaffs/getSet.cpp
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		The file containing all the boring getter and setter stuff
 * @copyright 	Copyright (c) 2022
 */

#include "leaffs.h"

std::string LeafFS::getError(){
	return _error;
}

std::deque<std::string> LeafFS::getFiles(){
	return _files;
}

std::deque<std::string> LeafFS::getDirectories(){
	return _directories;
}
