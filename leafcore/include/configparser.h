#ifndef __CONFIGPARSER_H__
#define __CONFIGPARSER_H__

#include <string>
#include <map>
#include <istream>

class ConfigParser{

public:
	ConfigParser();
	~ConfigParser();

	/**
	 * @brief	Parses the config provided by the stream
	 * @param	in				The stream to parse the config from
	 */
	void						parse(std::istream& in);

#ifndef FRIEND_CONFIGPARSER
private:
#endif

	//A map containing all the configs
	std::map<std::string, std::string>		_configs;

};

#endif
