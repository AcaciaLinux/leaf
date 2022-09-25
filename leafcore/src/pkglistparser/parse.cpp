#define FRIEND_PACKAGE
/**
 * @file		pkglistparser/parse.cpp
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		The implementation of PackageListParser::parse()
 * @copyright 	Copyright (c) 2022
 */

#include "log.h"
#include "error.h"
#include "leafdebug.h"
#include "pkglistparser.h"

void PackageListParser::parse(std::istream& in){
	FUN();

	LEAF_DEBUG_EX("PackageListParser::parse()");

	if (!in.good())
		throw new LeafError(Error::PKGPRS_BAD_STREAM);

	std::string line;
	std::deque<std::string> blocks;
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

		if (blocks.size() != 6){
			LOGE("Line \"" + line + "\" has invalid block count " + std::to_string(blocks.size()) + "/6");
			continue;
		}

		{//Log the result
			std::string logbuf = "Parsed line:";
			for (std::string block : blocks){
				logbuf += " {" + block + "}";
			}
			LOGF(logbuf);
		}

		Package* newPackage = new Package(blocks.at(0), blocks.at(2));
		newPackage->_realVersion = stoi(blocks.at(1));
		newPackage->_description = blocks.at(3);
		newPackage->_dependencies = parseDependenciesString(blocks.at(4));

		newPackage->_fetchURL = blocks.at(5);
		if (newPackage->_fetchURL.empty()){
			LOGI(newPackage->getFullName() + " gets treated as collection");
			newPackage->setIsCollection(true);
		}

		_packages.push_back(newPackage);
	}
}
