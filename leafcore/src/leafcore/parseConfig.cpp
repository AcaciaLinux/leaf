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
	configFile.open(_config.configFile());

	if (configFile.is_open()){
		_configParser.parse(configFile);

		_config.setRootDir(_config.rootDir + _configParser.get("root", ""));

		//If the root is not "/", check for the chroot-cmd config and check for the variables
		_config.chroot_cmd = _configParser.get("chroot-cmd", _config.chroot_cmd);
		if (_config.chroot_cmd.find("{ROOTDIR}") == _config.chroot_cmd.npos)
			throw new LeafError(Error::CONFF_INV_CONF, "Missing {ROOTDIR} at 'chroot-cmd'");
		if (_config.chroot_cmd.find("{COMMAND}") == _config.chroot_cmd.npos)
			throw new LeafError(Error::CONFF_INV_CONF, "Missing {COMMAND} at 'chroot-cmd'");

		_config.pkgListURL = _configParser.get("pkgListURL", _config.pkgListURL);
		_config.extDownloadCache = _configParser.get("downloadCache", _config.extDownloadCache);
	} else {
		LOGUW("Leafcore: No config file found, using command line config");
	}

	if (_config.rootDir != "/")
		LOGU("Directing leaf actions to " + _config.rootDir);
}
