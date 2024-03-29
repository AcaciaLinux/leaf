/**
 * @file		package/getSet.cpp
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		The file containing all the boring getter and setter stuff
 * @copyright 	Copyright (c) 2022
 */

#include "package.h"
#include "leafconfig.h"
#include "error.h"

std::deque<std::string>& Package::getProvidedFiles(){
	return _provided_files;
}

std::deque<std::string>& Package::getDependencies(){
	return _dependencies;
}

void Package::setName(std::string name){
	_name = name;
}

std::string Package::getName(){
	return _name;
}

std::string Package::getFullName(){
	return _name + "-" + _versionString;
}

void Package::setRealVersion(uint32_t v){
	_realVersion = v;
}

uint32_t Package::getRealVersion(){
	return _realVersion;
}

void Package::setVersion(std::string version){
	_versionString = version;
}

std::string Package::getVersion(){
	return _versionString;
}

void Package::setDescription(std::string description){
	_description = description;
}

std::string Package::getDescription(){
	return _description;
}

void Package::setFetchURL(std::string url){
	_fetchURL = url;
}

std::string Package::getFetchURL(){
	return _fetchURL;
}

std::deque<Package*> Package::getDependentPackages(){
	return _dependent_package;
}

void Package::setDB(LeafDB* db){
	_db = db;
}

LeafDB* Package::getDB(){
	return _db;
}

void Package::setIsCollection(bool s){
	_isCollection = s;
}

bool Package::isCollection(){
	return _isCollection;
}

std::string Package::getDownloadPath(){
	//Check if the database is ok
	if (_db == nullptr)
		throw new LeafError(Error::NODB);
	
	return _db->getCore()->getConfig().cacheDir() + "downloads/" + getFullName() + ".leafpkg";
}

std::string Package::getExtractedDir(){
	//Check if the database is ok
	if (_db == nullptr)
		throw new LeafError(Error::NODB);
	
	return _db->getCore()->getConfig().packagesDir() + getFullName() + "/";
}

std::string Package::getInstalledFilePath(){
	//Check if the database is ok
	if (_db == nullptr)
		throw new LeafError(Error::NODB);
	
	return _db->getCore()->getConfig().installedDir() + getName() + ".leafinstalled";
}

std::string Package::toString(){
	std::string buf = "Package ";
	buf += _name + _versionString + " (" + std::to_string(_realVersion) + ") " + "(" + _description + ")";
	for (std::string dep : _dependencies)
		buf += " [" + dep + "]";
	buf += " " + _fetchURL;

	return buf;
}
