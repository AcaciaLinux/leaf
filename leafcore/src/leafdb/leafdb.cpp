#include "log.h"
#include "leafdb.h"

LeafDB::LeafDB(){
	FUN();
}

LeafDB::~LeafDB(){
	FUN();

	for (auto entry : _packages){
		LOGD("Removing package " + entry.first);
		delete entry.second;
	}

	_packages.clear();
}