/**
 * @file		configparser/parse.cpp
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		The implementation of ConfigParser::parse()
 * @copyright	Copyright (c) 2022
 */
#include "log.h"
#include "error.h"
#include "leafdebug.h"
#include "leafconfig.h"

#include "configparser.h"

#include <algorithm>

void ConfigParser::parse(std::istream& in){
	FUN();

	LEAF_DEBUG_EX("ConfigParser::parse()");

	if (!in.good())
		throw new LeafError(Error::CFGPRS_BAD_STREAM);

	std::string line, name, value;
	size_t delimiterPos = 0;

	while(getline(in, line)){

		line.erase(std::remove_if(line.begin(), line.end(), isspace), line.end());

		if (line[0] == '#'){
			LOGF("ConfigParser: Ignoring commented line \"" + line + "\"");
			continue;
		}

		if (line.empty())
			continue;

		delimiterPos = 0;
		delimiterPos = line.find('=');

		if(delimiterPos == line.npos){
			LOGF("ConfigParser: Ignoring line without delimiter \"" + line + "\"");
			continue;
		}

		if(delimiterPos == 0){
			LOGE("ConfigParser: Config value without name not allowed in line \"" + line + "\"");
			continue;
		}

		if (delimiterPos == line.length()-1){
			LOGE("ConfigParser: Config name without value not allowed in line \"" + line + "\"");
			continue;
		}

		name = line.substr(0, delimiterPos);
		value = line.substr(delimiterPos+1);
		this->_configs[name] = value;

		LOGF("ConfigParser: Added config \"" + name + "\" -> \"" + this->_configs[name] + "\"");
	}
}
