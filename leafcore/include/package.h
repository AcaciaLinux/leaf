#ifndef __PACKAGE_H__
#define __PACKAGE_H__

class Package;

#include "file.h"
#include "leafdb.h"
#include "leafpkg.h"

#include <string>
#include <deque>
#include <unordered_map>
#include <ostream>
#include <istream>
class Package{

public:
	Package(std::string name, std::string version);
	static Package* CreateFromLocal(std::string path);

	/**
	 * @brief	Adds the supplied file path to the supplied files by the package
	 * @param	filenPath		The path to the file relative to root (/) that the package provides
	 */
	void						addProvidedFile(std::string filenPath);

	/**
	 * @brief	Outputs the .leafinstalled file from the package
	 * @param	out				The output stream to write to
	 */
	void						createInstalledFile(std::ostream& out);

	/**
	 * @brief	Parse a .leafinstalled file to this package, everything will be overwritten
	 * @param	in				The input stream to parse
	 */
	void						parseInstalledFile(std::istream& in);

	/**
	 * @brief	Fetches the package .lfpkg from the server
	 */
	void						fetch();

	/**
	 * @brief	Extracts the package into the package cache for deployment
	 */
	void						extract();

	/**
	 * @brief	Indexes the files that should be copied to the root
	 */
	void						indexExtracted();

	/**
	 * @brief	Copies all the files of the package to the root
	 * @param	forceOverwrite	If existing files should be overwritten
	 */
	void						copyToRoot(bool forceOverwrite = false);

	/**
	 * @brief	Deploys the package to the root, uses the following steps:
	 * 				runPreinstall()
	 * 				copyToRoot()
	 * 				createInstalledFile()
	 * 				runPostinstall()
	 */
	void						deploy();

	/**
	 * @brief	Runs the preinstall.sh script if it exists
	 */
	void						runPreinstall();

	/**
	 * @brief	Runs the postinstall.sh script if it exists
	 */
	void						runPostinstall();

	/**
	 * @brief	Runs the specified script if it is availabl
	 * @param	path			The path to the script relative to the package extracted directory
	 */
	void						runScript(std::string path);

	/**
	 * @brief	Removes this package from the root
	 */
	void						removeFromRoot();

	/**
	 * @brief	Removes the cached .lfpkg file if present
	 */
	void						removeDownloadCache();

	/**
	 * @brief	Clears the extracted cache of the package
	 */
	void						clearCache();

	/**
	 * @brief	Resolves a deque of packages that depend on this package
	 */
	bool						resolveDependentPackages();

	/**
	 * @brief	Checks if this package depends on the supplied package
	 * @param	package			The package to check
	 * @return	true if this package depends on the supplied package
	 */
	bool						dependsOn(Package* package);

	/**
	 * @brief	Returns a reference to the files provided by this package
	 */
	std::deque<std::string>&	getProvidedFiles();

	/**
	 * @brief	Returns a reference to the dependencies needed by this package
	 */
	std::deque<std::string>&	getDependencies();

	/**
	 * @brief	The name of the package
	 */
	void						setName(std::string);
	std::string					getName();

	/**
	 * @brief	The real version of the package
	 */
	void						setRealVersion(uint32_t);
	uint32_t					getRealVersion();
	
	/**
	 * @brief	Returns the package name in the following format: "name-version":
	 * 			eg: "mypackage-12.31.3"
	 */
	std::string					getFullName();

	/**
	 * @brief	The version string of the package
	 */
	void						setVersion(std::string);
	std::string					getVersion();

	/**
	 * @brief	The description of the package
	 */
	void						setDescription(std::string);
	std::string					getDescription();

	/**
	 * @brief	The fetch URL the package can be received from
	 */
	void						setFetchURL(std::string);
	std::string					getFetchURL();

	/**
	 * @brief	Applies the data supplied by a leaf.pkg file
	 * @param	lfpkg			The struct to apply
	 */
	void						applyLeafPkg(leafpkg_t lfpkg);

	/**
	 * @brief	The database the package belongs to
	 */
	void						setDB(LeafDB* db);
	LeafDB*						getDB();

	/**
	 * @brief	If this package represents a collection and thus has no fetch URL and no .leafinstalled
	 */
	void						setIsCollection(bool);
	bool						isCollection();

	/**
	 * @brief	Returns the destination for the package download
	 */
	std::string					getDownloadPath();

	/**
	 * @brief	Get the directory the extracted package contents should be in
	 */
	std::string					getExtractedDir();

	/**
	 * @brief	Get the path to the .leafinstalled file
	 */
	std::string					getInstalledFilePath();

	/**
	 * @brief	Returns a deque to the packages that depend on this package
	 */
	std::deque<Package*>		getDependentPackages();

	/**
	 * @brief	Returns the path to the locally installed .lfpkg file
	 */
	std::string					getLocalSourcePath();

	/**
	 * @brief	Checks if the provided file is provided by the package
	 * @param	filePath		The filepath to the file being checked relative to root (/)
	 */
	bool						checkFileProvided(std::string filePath);

	/**
	 * @brief	Checks the fetched hash against the remote hash
	 * @param	noThrow			If this function should not throw, return false instead
	 */
	bool						checkFetchedHash(bool noThrow = false);

	/**
	 * @brief	Clears all the contents of this package
	 */
	void						clear();

	/**
	 * @brief	Returns the package described in a string 
	 */
	std::string					toString();

#ifndef FRIEND_PACKAGE
private:
#endif

	/**
	 * @brief	The default constructor
	 */
	Package();

	/**
	 * @brief	A pointer to the database the package belongs to
	 */
	LeafDB*						_db = nullptr;

	/**
	 * @brief	The name of the package
	 */
	std::string					_name;

	/**
	 * @brief	The realversion of the package in u32
	 */
	uint32_t					_realVersion;

	/**
	 * @brief	The version string of the package
	 */
	std::string					_versionString;

	/**
	 * @brief	The description of the package
	 */
	std::string					_description;

	/**
	 * @brief	A deque containing all the dependency names for the package
	 */
	std::deque<std::string>		_dependencies;

	/**
	 * @brief	The URL to fetch the package from
	 */
	std::string					_fetchURL;

	/**
	 * @brief	The MD5 hash that is expected when fetching the package file
	 */
	std::string					_remote_md5;

	/**
	 * @brief	The MD5 hash of the present package file (local or in cache)
	 */
	std::string					_local_md5;

	/**
	 * @brief	The MD5 hash of the currently installed package file (stored in .leafinstalled)
	 */
	std::string					_installed_md5;

	/**
	 * @brief	Whether the package represents a collection of other packages
	 */
	bool						_isCollection = false;

	/**
	 * @brief	Whether the package is a local .lfpkg file
	 */
	bool						_isLocal = false;

	/**
	 * @brief	The path to the local .lfpkg file
	 */
	std::string					_localSourcePath;
	
	/**
	 * @brief	List of the provided files
	 */
	std::deque<std::string>		_provided_files;

	/**
	 * @brief	List of the provided directories
	 */
	std::deque<std::string>		_provided_directories;

	//
	// The .leafinstalled file is versioned, the functions for the versions
	//

	/**
	 * @brief	The parser function for .leafinstalled files in version 0
	 */
	void						parseInstalledV0(std::istream& in);

	/**
	 * @brief	The parser function for .leafinstalled files in version 1
	 */
	void						parseInstalledV1(std::istream& in);

	//
	// Some utility functions for parsing the .leafinstalled file
	//

	/**
	 * @brief	Parses a line containing an integer from the in stream
	 * @param	in				The input stream to parse from
	 * @param	description		The name of the entry, for the error if an unexpected EOF is encountered
	 */
	size_t						parseInstalledInt(std::istream& in, const std::string& description);

	/**
	 * @brief	Parses a line containing an string from the in stream
	 * @param	in				The input stream to parse from
	 * @param	description		The name of the entry, for the error if an unexpected EOF is encountered
	 */
	std::string					parseInstalledString(std::istream& in, const std::string& desciption);

	/**
	 * @brief	Parses a deque containing <count> lines of strings
	 * @param	in				The input stream to parse from
	 * @param	count			The amount of lines to parse into the deque
	 * @param	description		The name of the entry, for the error if an unexpected EOF is encountered
	 */
	std::deque<std::string>		parseInstalledList(std::istream& in, size_t count, const std::string& description);
};

#endif
