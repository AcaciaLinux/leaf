/**
 * @file		leafcore/parseConfig.cpp
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		The implementation of Leafcore::parseConfig()
 * @copyright	Copyright (c) 2022
 */
#include "log.h"
#include "error.h"
#include "leafdebug.h"

#include "leafcore.h"

#include <fstream>

//TODO: Tests

void Leafcore::parseConfig(){
	FUN();
	LEAF_DEBUG_EX("Leafcore::parseConfig()");

	std::ifstream configFile;
	configFile.open(_config.rootDir + "etc/leaf/leaf.conf");

	if (configFile.is_open()){
		_configParser.parse(configFile);

		_config.setRootDir(_config.rootDir + _configParser.get("root", ""));
		_config.pkgListURL = _configParser.get("pkgListURL", _config.pkgListURL);
	} else {
		LOGUW("Leafcore: No config file found, using command line config");
	}

	if (_config.rootDir != "/")
		LOGU("Directing leaf actions to " + _config.rootDir);
}
