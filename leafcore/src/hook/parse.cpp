/**
 * @file		hook/parse.cpp
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		The implementation of Hook::parse()
 * @copyright	Copyright (c) 2022
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

void Hook::parse(std::string inFile){
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

	std::string line, name, value;
	size_t delimiterPos = 0;
	std::map<std::string, std::string> entries;

	while(getline(in, line)){

		//Find the delimiter
		delimiterPos = 0;
		delimiterPos = line.find('=');

		//If the delimiter was not found
		if(delimiterPos == line.npos){
			LOGF("[Hook][parse] Ignoring line without delimiter \"" + line + "\"");
			continue;
		}

		//Strip all whitespaces except these of the config value
		line.erase(std::remove_if(line.begin(), line.begin()+delimiterPos, isspace), line.begin()+delimiterPos);
		
		//Refind the delimiter since it got moved
		delimiterPos = line.find('=');

		//Ignore commented lines
		if (line[0] == '#'){
			LOGF("[Hook][parse] Ignoring commented line \"" + line + "\"");
			continue;
		}

		//Ignore empty lines
		if (line.empty())
			continue;

		//If no name was specified for the value
		if(delimiterPos == 0){
			LOGE("[Hook][parse] Entry value without name not allowed in line \"" + line + "\"");
			continue;
		}

		//If no value was specified for a name
		if (delimiterPos == line.length()-1){
			LOGE("[Hook][parse] Entry name without value not allowed in line \"" + line + "\"");
			continue;
		}

		//Get the name and the value
		name = line.substr(0, delimiterPos);
		value = line.substr(delimiterPos+1);

		//Remove leading and trailing whitespaces
		value = std::regex_replace(value, std::regex("^ +| +$|( ) +"), "$1");

		//Store them in the entries
		entries[name] = value;

		//Log the result
		LOGF("[Hook][parse] Added entry \"" + name + "\" -> \"" + entries[name] + "\"");
	}

	apply(entries);

	_valid = true;

}
