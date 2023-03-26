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

#include <nlohmann/json.hpp>

void PackageListParser::parse(std::istream& in){
	FUN();

	LEAF_DEBUG_EX("PackageListParser::parse()");

	if (!in.good())
		throw new LeafError(Error::PKGPRS_BAD_STREAM);

	//Parse the json data and throw an error if parsing failed
	nlohmann::json json;
	try {
		json = nlohmann::json::parse(in, nullptr, true);
	} catch (nlohmann::json::parse_error& e) {
		throw new LeafError(Error::JSON_PARSE, e.what());
	}

	try {

		//Go over every package
		for(nlohmann::json json_pkg : json){
			LOGD("Found package: " + std::string(json_pkg["name"]));
		
			//Construct the new package
			Package* newPackage = new Package(json_pkg.at("name"), json_pkg.at("version"));

			//Fill the new package
			newPackage->_realVersion = std::stoi(std::string(json_pkg.at("real_version")));
			newPackage->_description = json_pkg.at("description");
			for (nlohmann::json json_dep : json_pkg.at("dependencies")){
				newPackage->_dependencies.push_back(std::string(json_dep));
			}

			//Check for the 'hash' tag
			if (json_pkg.count("hash") != 0)
				newPackage->_remote_md5 = json_pkg.at("hash");
			else
				LOGD("[PackageListParser][parse] Package " + std::string(json_pkg.at("name")) + " does lack the 'hash' entry");

			//Check for a fetchURL, else it is a collection
			newPackage->_fetchURL = json_pkg.at("url");
			if (newPackage->_fetchURL.empty()){
				LOGI(newPackage->getFullName() + " gets treated as collection");
				newPackage->setIsCollection(true);
			}

			//Add the new package to the database
			_packages.push_back(newPackage);

		}

	} catch (nlohmann::json::out_of_range& e){
		throw new LeafError(Error::JSON_OUT_OF_RANGE, e.what());
	}  catch (nlohmann::json::exception& e) {
		throw new LeafError(Error::JSON_PARSE, e.what());
	}
}
