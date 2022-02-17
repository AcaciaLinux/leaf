#ifndef __LEAF_FS_H__
#define __LEAF_FS_H__

#include <string>
#include <deque>

/**
 * @brief	Provides a filesystem abstraction useful for leaf to
 *			maintain the control over the file system leaf needs to have
 */

class LeafFS{

public:
	/**
	 * @brief	Initialize LeafFS
	 * @param	directory		The directory LeafFS should work with
	 */
	LeafFS(std::string directory);

	/**
	 * @brief	Checks if the directory can be used to operate on:
	 * 				If it exists
	 * 				If it really IS a directory
	 */
	bool						check();

	/**
	 * @brief	Reads all the files in the directory, get them using getFiles()
	 * @param	onlyfiles		Only include files excluding directories
	 * @param	recursive		Read recursively
	 */
	bool						readFiles(bool onlyfiles, bool recursive);

	/**
	 * @brief	Returns a reference to the previously read files in the directory
	 */
	std::deque<std::string>&	getFiles();

	/**
	 * @brief	Returns the last error
	 */
	std::string					getError();

private:

	//Holds the last error in clear readable form
	std::string					_error;

	//The currently selected directory
	std::string					_curDir;

	//The files contained in this directory parsed by readFiles() and readFilesRecursive()
	std::deque<std::string>		_files;

	bool						getFiles(std::string prefix, std::string directory, bool recursive, bool filesonly);
};

#endif