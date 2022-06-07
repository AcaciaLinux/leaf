#ifndef __HOOK_H__
#define __HOOK_H__

#include "leafconfig.h"

#include <deque>
#include <map>
#include <string>

enum hook_exec_time{
	HOOK_EXEC_PRE,
	HOOK_EXEC_POST
};

class Hook{

public:
	Hook();
	~Hook();

	/**
	 * @brief	Parses the hook description provided by the in stream
	 * @param	inFile			The path to the file to get parsed
	 */
	void						parse(std::string inFile);

#ifndef FRIEND_HOOK
private:
#endif

	/**
	 * @brief	If the hook has been parsed and validated successfully
	 */
	bool						_valid = false;

	/**
	 * @brief	The path to the file that contains the description for this hook
	 */
	std::string					_filePath;

	/**
	 * @brief	A deque of the actions this hook should react to
	 */
	std::deque<leaf_action>		_actions;

	/**
	 * @brief	A deque of the packages this hook should react to
	 */
	std::deque<std::string>		_packages;

	/**
	 * @brief	The time of installation this hook should get executed
	 */
	hook_exec_time				_execTime;

	/**
	 * @brief	A deque of the dependencies needed for the exec command 
	 */
	std::deque<std::string>		_runDeps;

	/**
	 * @brief	The command to get executed when the hook engages
	 */
	std::string					_exec;

	/**
	 * @brief	Applies the supplied map to the internal values
	 * @param	entries			A reference to the entries to parse
	 */
	void						apply(std::map<std::string, std::string>& entries);
};

#endif
