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
	void						check();

	/**
	 * @brief	Reads all the entries in the directory, get them using getFiles()
	 * @param	recursive		Read recursively
	 */
	void						read(bool recursive);

	/**
	 * @brief	Returns the previously read files in the directory
	 */
	std::deque<std::string>		getFiles();

	/**
	 * @brief	Returns all the directories in the filesystem
	 */
	std::deque<std::string>		getDirectories();

private:

	//The currently selected directory
	std::string					_curDir;

	//The directories in the filesystem parsed by readFiles()
	std::deque<std::string>		_directories;

	//The files contained in this directory parsed by readFiles() and readFilesRecursive()
	std::deque<std::string>		_files;

	void						getFiles(std::string prefix, std::string directory, bool recursive);
};

void							removeFile(std::string path, bool errorOnNotExisting);

#endif