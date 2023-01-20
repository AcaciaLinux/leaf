#ifndef __TESTING_FILES_H__
#define __TESTING_FILES_H__

#include <string>

class FSError{
public:
	FSError(std::string message);
	FSError(std::string message, std::error_code ec);

	std::string 		what();

private:
	std::string			_message;
	std::error_code		_ec;
};

/**
 * @brief	Checks if the path exists, else it will call FAIL()
 * @param	path				The path to check if it exists
 */
bool pathExists(std::string path);

/**
 * @brief	Creates a new file
 * @param	path				The path to the new file
 * @param	override			If the file should be overridden if existing (false will throw FSError)
 */
void createFile(std::string path, bool override = false);

/**
 * @brief	Creates a new directory
 * @param	path				The path to the new directory
 * @param	override			If the file should be overridden if existing (false will throw FSError)
 */
void createDir(std::string path, bool override = false);

/**
 * @brief	Deletes the specified filesystem entrys
 * @param	path				The path to delete
 */
void deletePath(std::string path);

#endif