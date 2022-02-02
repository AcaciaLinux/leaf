#ifndef __LEAFDB_H__
#define __LEAFDB_H__

#include <unordered_map>
#include <vector>

#include "file.h"
#include "package.h"

class LeafDB{

public:
	LeafDB();
	~LeafDB();

	/**
	 * @brief	Creates a new package within this db
	 * @arg		name				The name of the new package
	 * @return	A pointer to the newly created package
	 */
	Package*						newPackage(std::string name);

	/**
	 * @brief	Returns the package with the specified name, nullptr if no package was found
	 * @param	name				The name of the package to get
	 */
	Package*						getPackage(std::string name);

	/**
	 * @brief	Returns a vector of all packages providing a file
	 * @param	filepath			The file path to search for
	 */
	std::vector<Package*>			findFileProviders(std::string filepath);

#ifdef FRIEND_LEAFDB
private:
#endif

	std::unordered_map<std::string, Package*>	_packages;
	std::unordered_map<std::string, File*>		_files;
};

#endif