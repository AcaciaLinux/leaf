/**
 * @file		hook/shouldEngage.cpp
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		The implementation of Hook::shouldEngage()
 * @copyright	Copyright (c) 2023 Max Kofler and the AcaciaLinux developers
 */
#include "log.h"
#include "error.h"
#include "leafdebug.h"

#include "hook.h"

bool Leaf::Hook::shouldEngage(const conf_tr& conf) const{
	FUN();

	bool actionMatch = false;
	bool packageMatch = false;

	for (std::string conf_package : conf.packages){
			if (checkPackage(conf_package)){
				packageMatch = true;
				break;
			}
	}

	for (conf_tr_type hook_action : _actions){
		if (hook_action == conf.tr_type){
			actionMatch = true;
			break;
		}
	}

	bool res = actionMatch & packageMatch;

	if (res)
		LOGI("[Hook][shouldEngage] Hook at " + _filePath + " should engage");
	else
		LOGI("[Hook][shouldEngage] Hook at " + _filePath + " should NOT engage");

	return res;
}
