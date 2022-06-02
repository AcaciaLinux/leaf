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

leaf_config_t lConfig;

Leafcore::Leafcore(){
	FUN();

	_packageListDB = new LeafDB(this);
	_installedDB = new LeafDB(this);

	std::ifstream configFile;
	configFile.open(lConfig.rootDir + "etc/leaf/leaf.conf");

	if (configFile.is_open()){
		_configParser.parse(configFile);

		_pkglistURL = _configParser.get("pkglist", "http://84.252.121.236/packages/leaf.pkglist");
	} else {
		LOGUW("Leafcore: No config file found, using command line config");
	}

}
