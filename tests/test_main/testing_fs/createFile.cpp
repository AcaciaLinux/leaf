#include "log.h"
#include "testing_fs.h"

#include <filesystem>
#include <fstream>

void createFile(std::string path, bool override){
	FUN();

	bool exists = pathExists(path);

	if (exists){
		if (override)
			deletePath(path);
		else
			throw new FSError("Could not create existing file " + path);

		LOGF("Overriding existing file " + path);
	}

	LOGF("Creating new file " + path);
	
	std::ofstream newFile;
	newFile.open(path, std::ios::out);
	if (!newFile.is_open())
		throw new FSError("Could not create file " + path);
	newFile.close();
}
