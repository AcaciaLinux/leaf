#ifndef __PACKAGE_LIST_PARSER_H__
#define __PACKAGE_LIST_PARSER_H__

#include <istream>

#include "leafdb.h"

class PackageListParser{

public:
	~PackageListParser();
	
	/**
	 * @brief	Parses the supplied in stream
	 * @param	in				The stream to parse
	 */
	bool						parse(std::istream& in);

	/**
	 * @brief	Applies the cached package list to the supplied leafdb
	 * @param	db				The database to apply the packages to
	 */
	bool						applyToDB(LeafDB& db);

	/**
	 * @brief	Returns the last error in string form
	 */
	std::string					getError();

private:
	std::string					_error;
	std::vector<Package*>		_packages;

	/**
	 * @brief	Parses the dependency string of a package list entry
	 * @return	A vector containing the names
	 */
	std::vector<std::string>	parseDependenciesString(std::string);
};

#endif