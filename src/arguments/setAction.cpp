#include "log.h"
#include "leafconfig.h"
#include "arguments.h"



bool Arguments::setAction(std::string a){
	FUN();

	if (a == "update")
		lConfig.action = ACTION_UPDATE;
	else if (a == "install")
		lConfig.action = ACTION_INSTALL;
	else if (a == "remove")
		lConfig.action = ACTION_REMOVE;
	else
		return false;

	return true;
}
