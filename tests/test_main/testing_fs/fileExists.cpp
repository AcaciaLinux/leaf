#include "log.h"
#include "testing_fs.h"

#include <filesystem>

bool pathExists(std::string path){
	FUN();
	bool exists = false;

	std::error_code ec;

	exists = std::filesystem::exists(path, ec);

	if (ec)
		throw new FSError("Failed to check " + path, ec);

	if (exists)
		LOGF(path + " exists");
	else
		LOGF(path + " does not exist");

	return exists;
}
