#include "log.h"
#include "leafconfig.h"
#include "arguments.h"

#include <iostream>

bool Arguments::switchVerbosity(uint8_t v){
	FUN();

	Log::stream_config* conf = hlog->getStreamConf(std::cout);

	if (conf == nullptr){
		LOGUE("Could not get config! There is a bug in the code, report it!");
		return false;
	}

	switch(v){
	case 0:
		_config.verbosity = CONFIG_V_DEFAULT;
		conf->loglevel = Log::U;
		break;

	case 1:
		_config.verbosity = CONFIG_V_VERBOSE;
		LOGU("Using verbose logging");
		conf->loglevel = Log::I;
		break;

	case 2:
		_config.verbosity = CONFIG_V_SUPERVERBOSE;
		LOGU("Using superverbose logging");
		conf->loglevel = Log::D;
		break;

	case 3:
		_config.verbosity = CONFIG_V_ULTRAVERBOSE;
		LOGU("Using ultraverbose logging");
		conf->loglevel = Log::MEM;
		break;

	default:
		LOGUE("Invalid verbosity " + std::to_string(v) + "/{0, 1, 2, 3}");
		return false;
	}

	return true;
}
