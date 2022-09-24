/**
 * @file		parser/getSet.cpp
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		The implementation of getter and setters of Parser
 * @copyright	Copyright (c) 2022
 */
#include "log.h"
#include "error.h"
#include "leafdebug.h"

#include "parser.h"

std::string Parser::get(const std::string& key, const std::string& def){
	FUN();

	if (_entries.count(key) == 0)
		return def;

	return _entries.at(key);
}
