/**
 * @file		leafcore/leafcore.cpp
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		The core implementation of Leafcore
 * @copyright 	Copyright (c) 2022
 */

#include "log.h"
#include "leafconfig.h"
#include "leafcore.h"

#include <fstream>

Log::Log* hlog = nullptr;

Leafcore::Leafcore(){
	FUN();

	_packageListDB = new LeafDB(this);
	_installedDB = new LeafDB(this);

	std::ifstream configFile;
	configFile.open(_config.rootDir + "etc/leaf/leaf.conf");

	if (configFile.is_open()){
		_configParser.parse(configFile);

		_config.setRootDir(_config.rootDir + _configParser.get("root", ""));
		_pkglistURL = _configParser.get("pkglist", "http://84.252.121.236/packages/leaf.pkglist");
	} else {
		LOGUW("Leafcore: No config file found, using command line config");
	}

}
