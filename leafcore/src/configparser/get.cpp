/**
 * @file		configparser/get.cpp
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		The implementation of ConfigParser::get()
 * @copyright	Copyright (c) 2022
 */
#include "log.h"
#include "error.h"
#include "leafdebug.h"
#include "leafconfig.h"

#include "configparser.h"

std::string ConfigParser::get(std::string name, std::string def){
	FUN();

	LEAF_DEBUG_EX("ConfigParser::get()");

	if (_configs.count(name) <= 0)
		return def;

	return _configs[name];
}
