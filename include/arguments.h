#ifndef __Arguments_H__
#define __Arguments_H__

#include <deque>
#include <string>

class Arguments{

public:

	/**
	 * @brief	Parses the supplied command line arguments into this object
	 * @param	argc			The argument count
	 * @param	argv			The argument vector
	 * @return	Success, false should terminate the program
	 */
	bool						parse(int argc, char** argv);

private:

	/**
	 * @brief	Uses the supplied string to determine the action to be performed
	 * @return	Success, false should terminate the program
	 */
	bool						setAction(std::string);

};

extern Arguments arguments;

#endif
