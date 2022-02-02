#include "log.h"
#include "leafdb.h"

Package* LeafDB::getPackage(std::string name){
	FUN();

	if (_packages.count(name) == 0)
		return nullptr;

	return _packages[name];
}