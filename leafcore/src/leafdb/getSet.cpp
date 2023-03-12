/**
 * @file		leafdb/getSet.cpp
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		The file containing all the boring getter and setter stuff
 * @copyright 	Copyright (c) 2022
 */

#include "leafdb.h"

Leafcore* LeafDB::getCore(){
	return _core;
}

std::string LeafDB::getError(){
	return _error;
}

const std::unordered_map<std::string, Package*>& LeafDB::getPackages(){
	return _packages;
}
