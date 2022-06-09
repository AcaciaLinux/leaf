/**
 * @file		leafcore/getSet.cpp
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		The file containing all the boring getter and setter stuff
 * @copyright	Copyright (c) 2022
 */

#include "log.h"
#include "leafcore.h"

void Leafcore::setPkgListURL(std::string url){
	FUN();
	LOGD("Leafcore: Setting package list url to \"" + url + "\"");

	_pkglistURL = url;
}

std::string Leafcore::getPkgListURL(){
	return _pkglistURL;
}

void Leafcore::setConfig(leaf_config_t config){
	FUN();
	LOGD("Leafcore: Updating main config struct");

	_config = config;
}

leaf_config_t& Leafcore::getConfig(){
	return _config;
}
