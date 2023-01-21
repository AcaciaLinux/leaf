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
		try {
			if (std::regex_match(packageName, std::regex(hookPackage))){
				engage = true;
				break;
			}
		} catch (std::regex_error& e) {
			LOGUW("WARNING: Regex error when trying to evaluate hook match '" + hookPackage + "': " + std::string(e.what()));
		}
	}

	return engage;

}
