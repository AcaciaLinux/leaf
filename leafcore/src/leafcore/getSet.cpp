/**
 * @file	getSet.cpp
 * @author	Max Kofler
 * @brief	The file containing all the boring getter and setter stuff
 * @copyright Copyright (c) 2022
 */

#include "leafcore.h"

void Leafcore::setRootPath(std::string path){
	_rootPath = path;
}

std::string Leafcore::getRootPath(){
	return _rootPath;
}

void Leafcore::setPkgListFile(std::string path){
	_pkglistFile = path;
}

std::string Leafcore::getPkgListFile(){
	return _pkglistFile;
}

void Leafcore::setPkgListURL(std::string url){
	_pkglistURL = url;
}

std::string Leafcore::getPkgListURL(){
	return _pkglistURL;
}

std::string Leafcore::getError(){
	return _error;
}
