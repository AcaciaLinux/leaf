/**
 * @file		leafcore/getSet.cpp
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		The file containing all the boring getter and setter stuff
 * @copyright	Copyright (c) 2022
 */

#include "leafcore.h"

void Leafcore::setRootDir(std::string dir){

	if (dir[dir.length()-1] != '/')
		dir += '/';

	this->_rootDir = dir;

	this->_cacheDir = _rootDir + "var/cache/leaf/";
	this->_leafDir = _rootDir + "etc/leaf/";
	this->_pkglistFile = _leafDir + "leaf.pkglist";
}

std::string Leafcore::getRootDir(){
	return _rootDir;
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

std::string Leafcore::getDownloadDir(){
	return _cacheDir + "downloads/";
}

std::string Leafcore::getPackagesDir(){
	return _cacheDir + "packages/";
}

std::string Leafcore::getDownloadPath(Package* package){
	return getDownloadDir() + package->getFullName() + ".tar.xz";
}

std::string Leafcore::getExtractedDirectory(Package* package){
	return getPackagesDir() + package->getFullName() + "/";
}

std::string Leafcore::getError(){
	return _error;
}
