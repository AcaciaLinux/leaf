#include "log.h"
#include "leafconfig.h"
#include "arguments.h"

bool Arguments::switchVerbosity(uint8_t v){
	FUN();

	switch(v){
	case 0:
		lConfig.verbosity = CONFIG_V_DEFAULT;
		hlog->setLevel(Log::U);
		break;

	case 1:
		lConfig.verbosity = CONFIG_V_VERBOSE;
		LOGU("Using verbose logging");
		hlog->setLevel(Log::I);
		break;

	case 2:
		lConfig.verbosity = CONFIG_V_SUPERVERBOSE;
		LOGU("Using superverbose logging");
		hlog->setLevel(Log::D);
		break;

	case 3:
		lConfig.verbosity = CONFIG_V_ULTRAVERBOSE;
		LOGU("Using ultraverbose logging");
		hlog->setLevel(Log::MEM);
		break;

	default:
		LOGUE("Invalid verbosity " + std::to_string(v) + "/{0, 1, 2, 3}");
		return false;
	}

	return true;
}
