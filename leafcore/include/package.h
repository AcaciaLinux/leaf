#ifndef __PACKAGE_H__
#define __PACKAGE_H__

class Package;

#include "file.h"
#include "leafdb.h"

#include <string>
#include <deque>
#include <unordered_map>
#include <ostream>
#include <istream>

namespace EC{
	namespace Package{
		enum ec{
			//general
			NODB,
			NOROOT,
			NOCONFDIR,
			CREATEFILE,
			CREATEDIR,
			REMOVEFILE,
			REMOVEDIR,
			FS_ERROR,

			//deploy()
			DEPLOY_OPENINSTALLEDFILE,
			DEPLOY_RUNPREINSTALL,
			DEPLOY_RUNPOSTINSTALL,

			//copyToRoot()
			COPYTOROOT_NOPACKAGE,
			COPYTOROOT_EXISTING_FILE,

			NONE
		};
	}
}

class Package{

public:
	Package(std::string name, std::string version);

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
	 * @brief	Fetches the package .leafpkg from the server
	 */
	bool						fetch();

	/**
	 * @brief	Extracts the package into the package cache for deployment
	 */
	bool						extract();

	/**
	 * @brief	Indexes the files that should be copied to the root
	 */
	bool						indexExtracted();

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
	bool						deploy();

	/**
	 * @brief	Runs the preinstall.sh script if it exists
	 */
	bool						runPreinstall();

	/**
	 * @brief	Runs the postinstall.sh script if it exists
	 */
	bool						runPostinstall();

	/**
	 * @brief	Runs the specified script if it is availabl
	 * @param	path			The path to the script relative to the package extracted directory
	 */
	bool						runScript(std::string path);

	/**
	 * @brief	Removes this package from the root
	 */
	bool						removeFromRoot();

	/**
	 * @brief	Clears the extracted cache of the package
	 */
	bool						clearCache();

	/**
	 * @brief	Resolves a deque of packages that depend on this package
	 */
	bool						resolveDependentPackages();

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
	 * @brief	Checks if the provided file is provided by the package
	 * @param	filePath		The filepath to the file being checked relative to root (/)
	 */
	bool						checkFileProvided(std::string filePath);

	/**
	 * @brief	Clears all the contents of this package
	 */
	void						clear();

	/**
	 * @brief	Returns the package described in a string 
	 */
	std::string					toString();

	/**
	 * @brief	Returns the last error thrown
	 */
	std::string					getError();

	/**
	 * @brief	Returns the last error code
	 */
	EC::Package::ec				getErrorCode();

#ifndef FRIEND_PACKAGE
private:
#endif

	LeafDB*						_db = nullptr;

	EC::Package::ec				_errorCode = EC::Package::NONE;
	std::string					_error;

	std::string					_name;
	std::string					_versionString;
	std::string					_description;
	std::deque<std::string>		_dependencies;
	std::string					_fetchURL;

	bool						_isCollection = false;
	
	std::deque<std::string>		_provided_files;
	std::deque<std::string>		_provided_directories;

	std::deque<Package*>		_dependent_package;
};

#endif
