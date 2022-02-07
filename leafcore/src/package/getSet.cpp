/**
 * @file		package/getSet.cpp
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		The file containing all the boring getter and setter stuff
 * @copyright 	Copyright (c) 2022
 */

#include "package.h"

std::vector<std::string>& Package::getProvidedFiles(){
	return _provided_files;
}

std::vector<std::string>& Package::getDependencies(){
	return _dependencies;
}

void Package::setName(std::string name){
	_name = name;
}

std::string Package::getName(){
	return _name;
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

std::string Package::toString(){
	std::string buf = "Package ";
	buf += _name + " (" + _description + ")";
	for (std::string dep : _dependencies)
		buf += " [" + dep + "]";
	buf += " " + _fetchURL;

	return buf;
}
