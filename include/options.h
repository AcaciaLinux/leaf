#ifndef __OPTIONS_H__
#define __OPTIONS_H__

#include <vector>

typedef struct{
	std::vector<std::string>		packages;
	bool							verbose = false;
}run_options;

extern run_options options;

#endif
