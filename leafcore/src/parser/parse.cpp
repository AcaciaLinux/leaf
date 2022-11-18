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

void Parser::parse(std::istream& inStream){
	FUN();
	LEAF_DEBUG_EX("Parser::parse()");

	std::string line, key, value;
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

		//Refind the delimiter since it got moved
		delimiterPos = line.find('=');

		//Ignore commented lines
		if (line[0] == '#'){
			LOGF("[Parser][parse] Ignoring commented line \"" + line + "\"");
			LEAF_DEBUG_EX("Parser::comment");
			continue;
		}

		//Ignore empty lines
		if (line.empty())
			continue;

		//If no name was specified for the value
		if(delimiterPos == 0){
			LOGE("[Parser][parse] Entry value without key not allowed in line \"" + line + "\"");
			LEAF_DEBUG_EX("Parser::nokey");
			continue;
		}

		//If no value was specified for a name
		if (delimiterPos == line.length()-1){
			LOGE("[Parser][parse] Entry name without value not allowed in line \"" + line + "\"");
			LEAF_DEBUG_EX("Parser::novalue");
			continue;
		}

		//Get the name and the value
		key = line.substr(0, delimiterPos);
		value = line.substr(delimiterPos+1);

		//Remove leading and trailing whitespaces from key
		key = std::regex_replace(key, std::regex("^ +| +$|( ) +"), "$1");

		//Remove leading and trailing whitespaces from value
		value = std::regex_replace(value, std::regex("^ +| +$|( ) +"), "$1");

		//Store them in the entries
		_entries[key] = value;

		//Log the result
		LOGF("[Parser][parse] Added entry \"" + key + "\" -> \"" + _entries[key] + "\"");
	}
}
