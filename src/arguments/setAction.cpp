#include "log.h"
#include "leafconfig.h"
#include "arguments.h"



bool Arguments::setAction(std::string a){
	FUN();

	if (a == "update")
		_config.action = ACTION_UPDATE;
	else if (a == "install")
		_config.action = ACTION_INSTALL;
	else if (a == "remove")
		_config.action = ACTION_REMOVE;
	else if (a == "installLocal")
		_config.action = ACTION_INSTALLLOCAL;
	else if (a == "upgrade")
		_config.action = ACTION_UPGRADE;
	else if (a == "listInstalled")
		_config.action = ACTION_LISTINSTALLED;
	else
		return false;

	return true;
}
