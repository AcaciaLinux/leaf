#include "log.h"
#include "testing_fs.h"

#include <filesystem>
#include <fstream>

void deletePath(std::string path){
	FUN();

	if (!pathExists(path))
		throw new FSError("Could not delete non existing path " + path);
	
	std::error_code ec;

	LOGF("Deleting " + path);
	std::filesystem::remove_all(path, ec);

	if (ec)
		throw new FSError("Could not delete " + path, ec);
}
