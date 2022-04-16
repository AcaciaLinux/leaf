/**
 * @file		leafpkgparser/leafpkgparser.cpp
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		The core implementation of LeafPkgParser
 * @copyright 	Copyright (c) 2022
 */

#include "log.h"
#include "leafdebug.h"
#include "leafpkgparser.h"

void LeafPkgParser::parse(std::istream& in){
	FUN();
	LEAF_DEBUG_EX("LeafPkgParser::parse()");

	if (in.bad())
		throw new LeafError(Error::BAD_ISTREAM, "leaf.pkg parser - parse");

	throw new LeafError(Error::FEATURE_NOT_IMPLEMENTED, "LeafPkgParser::parse()");
}
