
#include <iostream>
#include <string>

#include <fstream>

#include "log.h"
#include "leafdb.h"
#include "arguments.h"

#include "downloader.h"
#include "pkglistparser.h"
#include "leafcore.h"

Arguments arguments;

Log::Log* hlog;

int main(int argc, char** argv){
	hlog = new Log::Log(Log::U);
	hlog->setFeature(Log::FEATURE_PRINTFUNNAMES, false);
	FUN();

	if (!arguments.parse(argc, argv)){
		return -1;
	}

	if (arguments.getAction() == ACTION_UPDATE){
		Leafcore leaf(arguments.getRootPath());

		if (!leaf.a_update()){
			LOGUE("Failed to update package list: " + leaf.getError());
			return -1;
		}

		return 0;
	} else if (arguments.getAction() == ACTION_INSTALL) {
		Leafcore leaf(arguments.getRootPath());
		leaf.parsePackageList();

		if (!leaf.a_install(arguments.getPackages())){
			LOGUE("Failed to install: " + leaf.getError());
		}
	}
}
