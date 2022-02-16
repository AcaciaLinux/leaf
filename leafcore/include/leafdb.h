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
	 * @a		version				The package version
	 * @return	A pointer to the newly created package
	 */
	Package*						newPackage(std::string name, std::string version);

	/**
	 * @brief	Adds the supplied package to the database
	 * @param	package				The package to add
	 * @return	false if the package exists
	 */
	bool							addPackage(Package* package);

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

	/**
	 * @brief	Resolves the dependencies of the provided package
	 * @param	package				The package to find the dependencies of
	 * @return	Empty vector if there was an error
	 */
	std::vector<Package*>			resolveDependencies(Package* package);
	
	/**
	 * @brief	The recursive version of resolveDependencies(Package*), gets used by it too
	 * @param	dependencies		A pointer to the dependencies vector to append to
	 * @param	package				The package to resolve the dependencies for
	 */
	bool							resolveDependencies(std::vector<Package*>* dependencies, Package* package);

	/**
	 * @brief	Removes all the packages from the database and the memory
	 */
	void							clear();

	/**
	 * @brief	Returns the last error in a string
	 */
	std::string						getError();

#ifdef FRIEND_LEAFDB
private:
#endif

	std::unordered_map<std::string, Package*>	_packages;
	std::unordered_map<std::string, File*>		_files;

	std::string									_error;
};

#endif