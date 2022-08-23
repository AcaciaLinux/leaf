/**
 * @file		leafcore/parseHooks.cpp
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		The implementation of Leafcore::parseHooks()
 * @copyright	Copyright (c) 2022
 */
#include "log.h"
#include "error.h"
#include "leafdebug.h"

#include "leafcore.h"
#include "leaffs.h"

#include <filesystem>

//TODO: Tests

void Leafcore::parseHooks(){
	FUN();
	LEAF_DEBUG_EX("Leafcore::parseHooks()");

	_hooks.clear();

	std::string hooksDir = _config.hooksDir();
	LeafFS fs(hooksDir);
	fs.read(true);

	auto allFiles = fs.getFiles();

	std::deque<std::string> hookFiles;
	for (std::string file : allFiles){
		if (std::filesystem::path(file).extension() == ".hook")
			hookFiles.push_back(hooksDir + file);
	}

	{	//Inform the user
		std::string logMsg;
		for (std::string hook : hookFiles)
			logMsg += "\n -> " + hook;
		LOGD("Found following hooks: " + logMsg);
	}

	for (std::string file : hookFiles){
		LOGD("Parsing hook at " + file + "...");
		Hook hook;
		hook.parse(file);
		_hooks.push_back(std::move(hook));
	}

}
