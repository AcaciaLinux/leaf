#include "log.h"
#include "leafdb.h"

std::vector<Package*> LeafDB::findFileProviders(std::string filepath){
	FUN();
	std::vector<Package*> providers;

	for(auto package : _packages){
		if (package.second == nullptr)
			continue;

		if (package.second->checkFileProvided(filepath))
			providers.push_back(package.second);
	}

	return providers;
}
