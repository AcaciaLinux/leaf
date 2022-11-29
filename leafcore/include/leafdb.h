#ifndef __LEAFDB_H__
#define __LEAFDB_H__

class LeafDB;

#include <unordered_map>
#include <deque>

#include "file.h"
#include "package.h"
#include "leafcore.h"

class LeafDB{

public:
	LeafDB(Leafcore* core);
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
	 * @brief	Updates the package name map entry for the specified oldName
	 * @param	oldName				The current name of the package
	 * @param	newName				The new name of the package
	 */
	bool							renamePackage(std::string oldName, std::string newName);

	/**
	 * @brief	Returns the package with the specified name, nullptr if no package was found
	 * @param	name				The name of the package to get
	 * @param	throwOnNotFound		If this function should throw a LeafError if the package could not be found
	 */
	Package*						getPackage(std::string name, bool throwOnNotFound);

	/**
	 * @brief	Returns a vector of all packages providing a file
	 * @param	filepath			The file path to search for
	 */
	std::deque<Package*>			findFileProviders(std::string filepath);

	/**
	 * @brief	Resolves the dependencies of the provided package
	 * @param	package				The package to find the dependencies of
	 * @return	Empty vector if there was an error
	 */
	std::deque<Package*>			resolveDependencies(Package* package);
	
	/**
	 * @brief	The recursive version of resolveDependencies(Package*), gets used by it too
	 * @param	dependencies		A pointer to the dependencies vector to append to
	 * @param	package				The package to resolve the dependencies for
	 */
	bool							resolveDependencies(std::deque<Package*>* dependencies, Package* package);

	/**
	 * @brief	Resolves the dependers of the provided package
	 * @param	package				The package to find the dependers of
	 * @return	Empty vector if there was an error
	 */
	std::deque<Package*>			resolveDependers(Package* package);
	
	/**
	 * @brief	Takes an iteration in resolving dependers for all packages
	 * @param	dependers			A reference to the dependers vector to append to
	 */
	void							resolveDependers(std::deque<Package*>& dependers);

	/**
	 * @brief	Returns the leaf core this database belongs to
	 */
	Leafcore*						getCore();

	/**
	 * @brief	Removes all the packages from the database and the memory
	 */
	void							clear();

	/**
	 * @brief	Returns the last error in a string
	 */
	std::string						getError();

#ifndef FRIEND_LEAFDB
private:
#endif

	Leafcore*									_core;

	std::unordered_map<std::string, Package*>	_packages;
	std::unordered_map<std::string, File*>		_files;

	std::string									_error;

	bool										fail(std::string message);
};

#endif