#include "log.h"
#include "leafdb.h"

Package* LeafDB::newPackage(std::string name){
	LOGD("Adding new package " + name);

	Package* pkg = new Package(name);
	_packages[name] = pkg;
	return pkg;
}