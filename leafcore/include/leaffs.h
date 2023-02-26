#ifndef __LEAF_FS_H__
#define __LEAF_FS_H__

#include <string>
#include <deque>
#include <filesystem>

enum LeafFS_Type{
	LEAFFS_DIR,
	LEAFFS_BLOCK_FILE,
	LEAFFS_CHAR_FILE,
	LEAFFS_FILE,
	LEAFFS_SOCKET,
	LEAFFS_SYMLINK,
	LEAFFS_FIFO,
	LEAFFS_OTHER,
	LEAFFS_EMPTY
};

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

	/**
	 * @brief	Returns if the specified filesystem entry does exist
	 * @param	path			The path to look up
	 * @param	canThrow		If this should throw an exception, else an error returns false
	 */
	static bool					exists(const std::filesystem::path& path, bool canThrow = true);

	/**
	 * @brief	A wrapper for std::filesystem::remove, throws an error or returns false
	 * @param	path			The path to remove
	 * @param	canThrow		If this should throw an exception, else an error returns false
	 * @return	The return value of the call
	 */
	static bool					remove(const std::filesystem::path& path, bool canThrow = true);

	/**
	 * @brief	A wrapper for std::filesystem::remove_all, throws an error or returns false
	 * @param	path			The path to remove
	 * @param	canThrow		If this should throw an exception, else an error returns false
	 * @return	The return value of the call
	 */
	static bool					remove_all(const std::filesystem::path& path, bool canThrow = true);

	/**
	 * @brief	A wrapper for std::filesystem::create_directory, throws an error or returns false (parent dir must exist)
	 * @param	path			The path to create
	 * @param	canThrow		If this should throw an exception, else an error returns false
	 * @return	The return value of the call
	 */
	static bool					create_directory(const std::filesystem::path& path, bool canThrow = true);

	/**
	 * @brief	A wrapper for std::filesystem::create_directories, throws an error or returns false
	 * @param	path			The path to create
	 * @param	canThrow		If this should throw an exception, else an error returns false
	 * @return	The return value of the call
	 */
	static bool					create_directories(const std::filesystem::path& path, bool canThrow = true);

	/**
	 * @brief	A wrapper for std::filesystem::current_path(), throws an error or returns an empty string
	 * @param	path			The path to change to
	 * @param	canThrow		If this should throw an exception, else an error returns an empty string
	 * @return	The return value of the call
	 */
	static std::string			get_workdir(bool canThrow = true);

	/**
	 * @brief	A wrapper for std::filesystem::current_path(path), throws an error or returns false
	 * @param	path			The path to change to
	 * @param	canThrow		If this should throw an exception, else an error returns false
	 * @return	The return value of the call
	 */
	static bool					change_workdir(const std::filesystem::path& path, bool canThrow = true);

	/**
	 * @brief	A wrapper for std::is_... calls, throws an error or returns false
	 * @param	path			The path to check
	 * @param	type			The filesystem type to check for
	 * @param	canThrow		If this should throw an exception, else an error returns false
	 * @return	The return value of the call
	 */
	static bool					is(const std::filesystem::path& path, LeafFS_Type type, bool canThrow = true);

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