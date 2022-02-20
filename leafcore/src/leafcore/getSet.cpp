/**
 * @file		leafcore/getSet.cpp
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		The file containing all the boring getter and setter stuff
 * @copyright	Copyright (c) 2022
 */

#include "leafcore.h"

void Leafcore::setPkgListURL(std::string url){
	_pkglistURL = url;
}

std::string Leafcore::getPkgListURL(){
	return _pkglistURL;
}

std::string Leafcore::getError(){
	return _error;
}
