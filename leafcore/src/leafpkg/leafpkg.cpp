/**
 * @file		leafpkg/leafpkg.cpp
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		The core implementation of LeafPkg::parse()
 * @copyright 	Copyright (c) 2022
 */

#include "log.h"
#include "leafdebug.h"
#include "leafpkg.h"
#include "parser.h"
#include "util.h"

namespace LeafPkg{

	leafpkg_t parse(std::istream& inStream){
		FUN();

		leafpkg_t lfpkg;

		Parser parser;
		parser.parse(inStream);

		lfpkg.name = parser.get("name", "");
		lfpkg.version = parser.get("version", "");

		try{
			lfpkg.real_version = std::stoi(parser.get("real_version", "0"));
		} catch (...) {
			lfpkg.real_version = 0;
			LOGUW("Could not parse real_version \"" + parser.get("real_version", "") + "\"");
		}

		lfpkg.description = parser.get("description", "");
		lfpkg.dependencies = LeafUtil::parseDependenciesString(parser.get("dependencies"));

		return lfpkg;
	}
}
