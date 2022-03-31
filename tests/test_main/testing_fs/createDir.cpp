#include "log.h"
#include "testing_fs.h"

#include <filesystem>
#include <fstream>

void createDir(std::string path, bool override){
	FUN();

	bool exists = pathExists(path);

	if (exists){
		if (override)
			deletePath(path);
		else
			throw new FSError("Could not create existing directory " + path);

		LOGF("Overriding existing directory " + path);
	}

	LOGF("Creating new directory " + path);
	
	std::error_code ec;
	std::filesystem::create_directories(path);

	if (ec)
		throw new FSError("Failed to create directory " + path, ec);
}
