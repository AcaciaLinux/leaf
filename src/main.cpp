
#include <iostream>
#include <string>

#include <fstream>

#include "log.h"
#include "leafconfig.h"
#include "error.h"
#include "leafdb.h"
#include "arguments.h"

#include "downloader.h"
#include "pkglistparser.h"
#include "leafcore.h"

#include <deque>

int main(int argc, char** argv){
	hlog = new Log::Log(Log::U);
	hlog->setFeature(Log::FEATURE_PRINTFUNNAMES, false);
	FUN();

	Arguments arguments;

	if (!arguments.parse(argc, argv)){
		return -1;
	}

	leaf_config_t lConfig = arguments.getConfig();

	try {
		if (lConfig.action == ACTION_UPDATE){
			Leafcore leaf;

			leaf.a_update();

			return 0;
		} else if (lConfig.action == ACTION_INSTALL) {
			Leafcore leaf;
			
			leaf.parsePackageList();

			leaf.parseInstalled();

			leaf.a_install(lConfig.packages);
		} else if (lConfig.action == ACTION_REMOVE){
			Leafcore leaf;

			leaf.a_remove(lConfig.packages);
		}
	} catch (LeafError* e){
		LOGUE("Failed with error code " + std::to_string(e->getErrorCode()) + ": " + e->what());
	} catch (std::exception* e){
		LOGUE("Failed with fatal exception: " + std::string(e->what()));
	} catch (...){
		LOGUE("Failed with unknown fatal exception");
	}
}
