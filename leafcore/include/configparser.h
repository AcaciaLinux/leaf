#ifndef __CONFIGPARSER_H__
#define __CONFIGPARSER_H__

#include <string>
#include <map>
#include <istream>

#include "parser.h"

class ConfigParser : public Parser{

public:
	/**
	 * @brief	Returns the found config if specifies, else the default
	 * @param	name			The name for the value in the config file
	 * @param	def				The value that is returned if the config was not specified (default: "")
	 * @return	std::string		The value if found, else the def argument
	 */
	std::string					get(std::string name, std::string def = "");

#ifndef FRIEND_CONFIGPARSER
private:
#endif

	//A map containing all the configs
	std::map<std::string, std::string>		_configs;

};

#endif
