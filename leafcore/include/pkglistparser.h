#ifndef __PACKAGE_LIST_PARSER_H__
#define __PACKAGE_LIST_PARSER_H__

#include <istream>
#include <string>
#include <deque>

#include "leafdb.h"

class PackageListParser{

public:
	PackageListParser();
	~PackageListParser();
	
	/**
	 * @brief	Parses the supplied in stream
	 * @param	in				The stream to parse
	 */
	void						parse(std::istream& in);

	/**
	 * @brief	Applies the cached package list to the supplied leafdb
	 * @param	db				The database to apply the packages to
	 */
	void						applyToDB(LeafDB& db);

#ifndef FRIEND_PKGLISTPARSER
private:
#endif

	std::deque<Package*>		_packages;

	/**
	 * @brief	Parses the dependency string of a package list entry
	 * @return	A vector containing the names
	 */
	std::deque<std::string>		parseDependenciesString(std::string);
};

#endif