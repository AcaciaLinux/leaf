#ifndef __PACKAGE_H__
#define __PACKAGE_H__

#include <string>

class Package{

public:
	Package(std::string name);

	/**
	 * @brief	Returns the name of the package
	 */
	std::string				getName();

	/**
	 * @brief	Returns the description of the package
	 */
	std::string				getDescription();



private:
	std::string				_name;
	std::string				_description;

};

#endif
