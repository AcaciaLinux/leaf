#include "log.h"

#define FRIEND_PACKAGE
#include "pkglistparser.h"

PackageListParser::~PackageListParser(){
	FUN();

	for (Package* package : _packages)
		delete package;
}

bool PackageListParser::parse(std::istream& in){
	FUN();

	if (!in.good()){
		_error = "Input stream is bad";
		LOGE("Failed to parse list: " + _error);
		return false;
	}

	std::string line;
	std::vector<std::string> blocks;
	std::string buf;

	while(getline(in, line)){
		LOGF("Parsing line \"" + line + "\"");

		blocks.clear();
		buf = "";
		
		//Parse the ';' separated string
		for (char c : line){
			if (c == ';'){
				blocks.push_back(buf);
				buf = "";
			}else
				buf += c;
		}

		//Push the last block
		blocks.push_back(buf);

		if (blocks.size() != 4){
			LOGE("Line \"" + line + "\" has invalid block count " + std::to_string(blocks.size()) + "/4");
			continue;
		}

		{//Log the result
			std::string logbuf = "Parsed line:";
			for (std::string block : blocks){
				logbuf += " {" + block + "}";
			}
			LOGD(logbuf);
		}

		Package* newPackage = new Package(blocks.at(0));
		newPackage->_description = blocks.at(1);
		newPackage->_dependencies = parseDependenciesString(blocks.at(2));
		newPackage->_fetchURL = blocks.at(3);

		_packages.push_back(newPackage);
	}

	return true;
}

bool PackageListParser::applyToDB(LeafDB& db){
	FUN();

	bool ret = true;
	std::string err = "Failed to apply packages";

	for (Package* package : _packages){
		if (!db.addPackage(package)){
			LOGE("Failed to add package " + package->getName() + ", already existing");
			ret = false;
			err += " {" + package->getName() + "}";
			
			//If the apply fails, clean the memory
			delete package;
		}
	}

	//Deploy a readable error message
	if (!ret){
		err += " to database";
		_error = err;
	}

	//Clear the packages vector, the DB now owns the packages and manages them
	_packages.clear();

	return ret;
}

std::string PackageListParser::getError(){
	return _error;
}

std::vector<std::string> PackageListParser::parseDependenciesString(std::string deps){
	FUN();

	std::string buf = "";
	std::vector<std::string> list;

	for (char c : deps){
		switch (c){
			case '[':
				buf = "";
				break;

			case ']':
				list.push_back(buf);
				buf = "";
				break;

			default:
				buf += c;
				break;
		}
	}

	return list;
}