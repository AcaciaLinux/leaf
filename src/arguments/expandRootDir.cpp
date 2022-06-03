#include "log.h"
#include "leafconfig.h"
#include "arguments.h"

#include <filesystem>

bool Arguments::expandRootDir(std::string root){
	FUN();

	std::error_code ec;
	std::string buf = std::filesystem::absolute(root, ec);

	if (ec){
		LOGE("Failed to get absolute path of " + root + ": " + ec.message());
		return false;
	}

	LOGD("Expanded absolute path of " + root + ": " + buf);
	_config.setRootDir(buf);

	return true;
}
