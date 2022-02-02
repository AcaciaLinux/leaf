#ifndef __LEAFDB_H__
#define __LEAFDB_H__

#include <unordered_map>

#include "file.h"
#include "package.h"

class LeafDB{

	

private:
	std::unordered_map<std::string, Package>	_packages;
	std::unordered_map<std::string, File>		_files;
};

#endif