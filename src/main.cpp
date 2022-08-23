
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

#ifdef LOG_ENABLE_PROFILING
#include <fstream>
#endif

int main(int argc, char** argv){
	hlog = new Log::Log(Log::U);
	hlog->setFeature(Log::FEATURE_PRINTFUNNAMES, false);

	#ifdef LOG_ENABLE_PROFILING
	std::ofstream* profileStream = new std::ofstream();
	profileStream->open("profile.json", std::ios::out);
	hlog->setFeature(Log::FEATURE_PROFILE, true);
	hlog->setProfileStream(profileStream);
	#endif

	{
		FUN();

		Arguments arguments;

		if (!arguments.parse(argc, argv)){
			return -1;
		}

		leaf_config_t lConfig = arguments.getConfig();

		try {
			Leafcore leaf;
			leaf.setConfig(lConfig);
			leaf.parseHooks();

			if (lConfig.action == ACTION_UPDATE){
				leaf.a_update();

				return 0;
			} else if (lConfig.action == ACTION_INSTALL) {
				leaf.parsePackageList();

				leaf.parseInstalled();

				leaf.a_install(lConfig.packages);
			} else if (lConfig.action == ACTION_REMOVE){
				leaf.a_remove(lConfig.packages);
			} else if (lConfig.action == ACTION_INSTALLLOCAL){
				leaf.parseInstalled();

				leaf.a_installLocal(lConfig.packages);
			}
		} catch (LeafError* e){
			LOGUE("Failed with error code " + std::to_string(e->getErrorCode()) + ": " + e->what());
		} catch (std::exception* e){
			LOGUE("Failed with fatal exception: " + std::string(e->what()));
		} catch (...){
			LOGUE("Failed with unknown fatal exception");
		}
	}

	delete hlog;
	
	#ifdef LOG_ENABLE_PROFILING
	delete profileStream;
	#endif
}
