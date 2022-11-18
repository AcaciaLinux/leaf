/**
 * @file		hook/checkPackage.cpp
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		The implementation of Hook::checkPackage()
 * @copyright	Copyright (c) 2022
 */
#include "log.h"
#include "error.h"
#include "leafdebug.h"

#include "hook.h"

#include <regex>

bool Hook::checkPackage(std::string& packageName){
	FUN();

	bool engage = false;

	for (std::string& hookPackage : _packages){
		if (std::regex_match(packageName, std::regex(hookPackage))){
			engage = true;
			break;
		}
	}

	return engage;

}
