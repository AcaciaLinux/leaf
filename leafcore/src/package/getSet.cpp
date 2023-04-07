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

std::string Package::getName() const{
	return _name;
}

std::string Package::getFullName() const{
	return _name + "-" + _versionString;
}

void Package::setRealVersion(uint32_t v){
	_realVersion = v;
}

uint32_t Package::getRealVersion() const{
	return _realVersion;
}

void Package::setVersion(std::string version){
	_versionString = version;
}

std::string Package::getVersion() const{
	return _versionString;
}

void Package::setDescription(std::string description){
	_description = description;
}

std::string Package::getDescription() const{
	return _description;
}

void Package::setFetchURL(std::string url){
	_fetchURL = url;
}

std::string Package::getFetchURL() const{
	return _fetchURL;
}

void Package::setIsCollection(bool s){
	_isCollection = s;
}

bool Package::isCollection() const{
	return _isCollection;
}

std::filesystem::path Package::getLocalSourcePath() const{
	return _localSourcePath;
}

void Package::set_remote_md5(const std::string& md5){
	_remote_md5 = md5;
}
std::string Package::get_remote_md5() const{
	return _remote_md5;
}

void Package::set_local_md5(const std::string& md5){
	_local_md5 = md5;
}
std::string Package::get_local_md5() const{
	return _local_md5;
}

void Package::set_installed_md5(const std::string& md5){
	_installed_md5 = md5;
}
std::string Package::get_installed_md5() const{
	return _installed_md5;
}

std::filesystem::path Package::getDownloadPath(const Leaf::config& conf) const{
	return conf.downloadDir().append(getFullName() + ".lfpkg");
}

std::filesystem::path Package::getExtractedDir(const Leaf::config& conf) const{
	return conf.packagesDir().append(getFullName());
}

std::filesystem::path Package::getInstalledFilePath(const Leaf::config& conf) const{
	return conf.downloadDir().append(getName() + ".leafinstalled");
}

std::string Package::toString() const{
	std::string buf = "Package ";
	buf += _name + "-" + _versionString + " (" + std::to_string(_realVersion) + ") " + "(" + _description + ")";
	for (std::string dep : _dependencies)
		buf += " [" + dep + "]";
	buf += " " + _fetchURL;

	return buf;
}
