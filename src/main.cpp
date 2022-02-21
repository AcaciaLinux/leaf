
#include <iostream>
#include <string>

#include <fstream>

#include "log.h"
#include "leafconfig.h"
#include "leafdb.h"
#include "arguments.h"

#include "downloader.h"
#include "pkglistparser.h"
#include "leafcore.h"

Log::Log* hlog;

#include <deque>

int main(int argc, char** argv){
	hlog = new Log::Log(Log::U);
	hlog->setFeature(Log::FEATURE_PRINTFUNNAMES, false);
	FUN();

	Arguments arguments;

	if (!arguments.parse(argc, argv)){
		return -1;
	}

	if (lConfig.action == ACTION_UPDATE){
		Leafcore leaf;

		if (!leaf.a_update()){
			LOGUE("Failed to update package list: " + leaf.getError());
			return -1;
		}

		return 0;
	} else if (lConfig.action == ACTION_INSTALL) {
		Leafcore leaf;
		
		if (!leaf.parsePackageList()){
			LOGUE("Failed to install: " + leaf.getError());
			return -1;
		}

		if (!leaf.parseInstalled()){
			LOGUE("Failed to install: " + leaf.getError());
			return -1;
		}

		if (!leaf.a_install(lConfig.packages, lConfig.redownload == CONFIG_REDOWNLOAD_SPECIFIED)){
			LOGUE("Failed to install: " + leaf.getError());
			return -1;
		}
	} else if (lConfig.action == ACTION_REMOVE){
		Leafcore leaf;

		if (!leaf.a_remove(lConfig.packages)){
			LOGUE("Failed to remove: " + leaf.getError());
			return -1;
		}
	}
}
