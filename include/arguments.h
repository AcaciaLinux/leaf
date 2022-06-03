#ifndef __Arguments_H__
#define __Arguments_H__

#include <deque>
#include <string>

#include "leafcore/leafconfig.h"

class Arguments{

public:

	/**
	 * @brief	Parses the supplied command line arguments into this object
	 * @param	argc			The argument count
	 * @param	argv			The argument vector
	 * @return	Success, false should terminate the program
	 */
	bool						parse(int argc, char** argv);

	/**
	 * @brief	Returns the parsed config
	 */
	leaf_config_t				getConfig();

private:

	/**
	 * @brief	Uses the supplied string to determine the action to be performed
	 * @return	Success, false should terminate the program
	 */
	bool						setAction(std::string);

	/**
	 * @brief	Sets the verbosity to the supplied level
	 * @return	Success, false should terminate the program
	 */
	bool						switchVerbosity(uint8_t);

	/**
	 * @brief	Uses the std::filesystem api to get the full absolute path to the provided path
	 * @param	root			The relative path from the leaf executable to look up
	 */
	bool						expandRootDir(std::string root);

	/**
	 * @brief	The parsed config in by the command line arguments
	 */
	leaf_config_t				_config;
};

#endif
