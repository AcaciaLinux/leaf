/**
 * @file		leafdb/getSet.cpp
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		The file containing all the boring getter and setter stuff
 * @copyright 	Copyright (c) 2022
 */

#include "pkglistparser.h"

std::string PackageListParser::getError(){
	return _error;
}
