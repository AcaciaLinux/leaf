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

	//Check if the instream is good
	if (!in.good())
		throw new LeafError(Error::CFGPRS_BAD_STREAM);

	std::string line, name, value;
	size_t delimiterPos = 0;

	while(getline(in, line)){

		//Find the delimiter
		delimiterPos = 0;
		delimiterPos = line.find('=');

		//If the delimiter was not found
		if(delimiterPos == line.npos){
			LOGF("ConfigParser: Ignoring line without delimiter \"" + line + "\"");
			continue;
		}

		//Strip all whitespaces except these of the config value
		line.erase(std::remove_if(line.begin(), line.begin()+delimiterPos, isspace), line.begin()+delimiterPos);
		
		//Refind the delimiter since it got moved
		delimiterPos = line.find('=');

		//Ignore commented lines
		if (line[0] == '#'){
			LOGF("ConfigParser: Ignoring commented line \"" + line + "\"");
			continue;
		}

		//Ignore empty lines
		if (line.empty())
			continue;

		//If no name was specified for the value
		if(delimiterPos == 0){
			LOGE("ConfigParser: Config value without name not allowed in line \"" + line + "\"");
			continue;
		}

		//If no value was specified for a name
		if (delimiterPos == line.length()-1){
			LOGE("ConfigParser: Config name without value not allowed in line \"" + line + "\"");
			continue;
		}

		//Get the name and the value
		name = line.substr(0, delimiterPos);
		value = line.substr(delimiterPos+1);

		//Store them in the configs
		this->_configs[name] = value;

		//Log the result
		LOGF("ConfigParser: Added config \"" + name + "\" -> \"" + this->_configs[name] + "\"");
	}
}
