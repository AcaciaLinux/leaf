#ifndef __Arguments_H__
#define __Arguments_H__

#include <vector>
#include <string>

enum e_action{
	ACTION_UPDATE,
	ACTION_INSTALL,
	ACTION_REMOVE
};

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
	 * @brief	Returns if the program should be verbose
	 */
	bool						getVerbose();

	/**
	 * @brief	Returns the action to perform
	 */
	e_action					getAction();

	/**
	 * @brief	Returns the root path leaf should use
	 */
	std::string					getRootPath();

	/**
	 * @brief	Returns the packages that should be operated on
	 */
	std::vector<std::string>	getPackages();

private:
	//If the program should be verbose
	bool						verbose = false;

	//The action to perform
	e_action					action;

	//The root path leaf should use (for development is ./root)
	std::string					rootPath = "./root/";

	//The packages to perform the action on
	std::vector<std::string>	packages;

	/**
	 * @brief	Uses the supplied string to determine the action to be performed
	 * @return	Success, false should terminate the program
	 */
	bool						setAction(std::string);

};

extern Arguments arguments;

#endif
