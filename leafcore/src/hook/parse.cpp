/**
 * @file		hook/parse.cpp
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		The implementation of Hook::parse()
 * @copyright	Copyright (c) 2023 Max Kofler and the AcaciaLinux developers
 */
#include "log.h"
#include "error.h"
#include "leafdebug.h"
#include "leafconfig.h"

#include "hook.h"

#include <fstream>
#include <algorithm>
#include <filesystem>
#include <map>
#include <regex>

void Leaf::Hook::parse(std::string inFile){
	FUN();
	LEAF_DEBUG_EX("Hook::parse()");

	_valid = false;
	_filePath = inFile;

	//Check if the file exists
	{
		std::error_code ec;
		bool exists = std::filesystem::exists(inFile, ec);

		if (ec)
			throw new LeafError(Error::FS_ERROR, "[Hook][parse] Checking for existance of \"" + inFile + "\"");
		
		if (!exists)
			throw new LeafError(Error::OPENFILER, "[Hook][parse] \"" + inFile + "\" does not exist");
	}

	std::ifstream in;
	in.open(inFile, std::ios::in);

	if (!in.is_open())
		throw new LeafError(Error::OPENFILER, "[Hook][parse] Could not open file \"" + inFile + "\"");

	Parser::parse(in);

	apply();

	_valid = true;

}
