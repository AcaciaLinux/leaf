#include "log.h"
#include "package.h"

Package::Package(std::string name){
	FUN();
	_name = name;
	LOGF("Created package \"" + name + "\"");
}

std::string Package::getName(){
	FUN();
	return _name;
}

std::string Package::getDescription(){
	FUN();
	return _description;
}

