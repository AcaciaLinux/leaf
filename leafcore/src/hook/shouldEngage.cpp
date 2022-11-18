/**
 * @file		hook/shouldEngage.cpp
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		The implementation of Hook::shouldEngage()
 * @copyright	Copyright (c) 2022
 */
#include "log.h"
#include "error.h"
#include "leafdebug.h"

#include "hook.h"

bool Hook::shouldEngage(leaf_config_t& config){
	FUN();

	bool actionMatch = false;
	bool packageMatch = false;

	for (std::string hook_package : _packages){
		for (std::string conf_package : config.packages)
			if (hook_package == conf_package){
				packageMatch = true;
				break;
			}
	}

	for (leaf_action hook_action : _actions){
		if (hook_action == config.action){
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
