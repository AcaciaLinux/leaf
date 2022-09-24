/**
 * @file		parser/parse.cpp
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		The implementation of Parser::parse()
 * @copyright	Copyright (c) 2022
 */
#include "log.h"
#include "error.h"
#include "leafdebug.h"

#include "parser.h"

#include <map>
#include <regex>

//TODO: Tests

void Parser::parse(std::istream& inStream){
	FUN();
	LEAF_DEBUG_EX("Parser::parse()");

	std::string line, name, value;
	size_t delimiterPos = 0;
	_entries.clear();

	while(getline(inStream, line)){

		//Find the delimiter
		delimiterPos = 0;
		delimiterPos = line.find('=');

		//If the delimiter was not found
		if(delimiterPos == line.npos){
			LOGF("[Parser][parse] Ignoring line without delimiter \"" + line + "\"");
			continue;
		}

		//Strip all whitespaces except these of the config value
		line.erase(std::remove_if(line.begin(), line.begin()+delimiterPos, isspace), line.begin()+delimiterPos);
		
		//Refind the delimiter since it got moved
		delimiterPos = line.find('=');

		//Ignore commented lines
		if (line[0] == '#'){
			LOGF("[Parser][parse] Ignoring commented line \"" + line + "\"");
			continue;
		}

		//Ignore empty lines
		if (line.empty())
			continue;

		//If no name was specified for the value
		if(delimiterPos == 0){
			LOGE("[Parser][parse] Entry value without name not allowed in line \"" + line + "\"");
			continue;
		}

		//If no value was specified for a name
		if (delimiterPos == line.length()-1){
			LOGE("[Parser][parse] Entry name without value not allowed in line \"" + line + "\"");
			continue;
		}

		//Get the name and the value
		name = line.substr(0, delimiterPos);
		value = line.substr(delimiterPos+1);

		//Remove leading and trailing whitespaces
		value = std::regex_replace(value, std::regex("^ +| +$|( ) +"), "$1");

		//Store them in the entries
		_entries[name] = value;

		//Log the result
		LOGF("[Parser][parse] Added entry \"" + name + "\" -> \"" + _entries[name] + "\"");
	}
}
