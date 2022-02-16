/**
 * @file		leafpkgparser/leafpkgparser.cpp
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		The core implementation of LeafPkgParser
 * @copyright 	Copyright (c) 2022
 */

#include "log.h"
#include "leafpkgparser.h"

bool LeafPkgParser::parse(std::istream& in){
	FUN();
	_error.clear();

	if (in.bad()){
		_error = "Stream is bad";
		LOGE("Failed to parse leafpkg: " + _error);
		return false;
	}

	return false;
}
