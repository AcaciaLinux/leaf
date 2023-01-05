/**
 * @file		package/parseInstalledFile.cpp
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		The implementation of Package::parseInstalledFile()
 * @copyright	Copyright (c) 2022
 */
#include "log.h"
#include "leafdebug.h"
#include "leafconfig.h"
#include "package.h"

void Package::parseInstalledFile(std::istream& in){
	FUN();

	LEAF_DEBUG_EX("Package::parseInstalledFile()");

	//TODO: tests

	if (!in.good())
		throw new LeafError(Error::BAD_ISTREAM, "parseInstalledFile() (This is a bug)");

	std::string versionLine = parseInstalledString(in, "version or name");

	size_t version = 0;
	try {
		version = std::stoi(versionLine);
	} catch (...) {
		LOGW("[Package][parseInstalledFile] Failed to convert version, assuming V0 file!");
		version = 0;

		//We just parsed the name, set it
		_name = versionLine;
	}

	switch (version){

		case 0:
			parseInstalledV0(in);
			break;

		case 1:
			parseInstalledV1(in);
			break;

		default:
			throw new LeafError(Error::PACKAGE_PARSE_INSTALLEDFILE, "Invalid version " + std::to_string(version));

	}
}

size_t Package::parseInstalledInt(std::istream& in, const std::string& description){
	FUN();
	std::string line;
	size_t res;

	if (!getline(in, line))
		throw new LeafError(Error::PACKAGE_UNEXPECTED_EOF, description);
	try{
		res = stoi(line);
	} catch (...){
		clear();
		throw new LeafError(Error::PACKAGE_STOI, line);
	}

	return res;
}

std::string Package::parseInstalledString(std::istream& in, const std::string& description){
	FUN();
	std::string res;

	if (!getline(in, res))
		throw new LeafError(Error::PACKAGE_UNEXPECTED_EOF, description);

	return res;
}

std::deque<std::string> Package::parseInstalledList(std::istream& in, size_t count, const std::string& description){
	FUN();
	std::string line;
	std::deque<std::string> res;

	for (int i = 0; i < count; i++){
		if (!getline(in, line))
			throw new LeafError(Error::PACKAGE_UNEXPECTED_EOF, description + " " + std::to_string(i) + "/" + std::to_string(count));

		res.push_back(line);
	}

	return res;
}

void Package::parseInstalledV0(std::istream& in){
	FUN();

	//_name has already been parsed before
	_realVersion = parseInstalledInt(in, "realversion");
	_versionString = parseInstalledString(in, "version");
	size_t countDependencies = parseInstalledInt(in, "dependency count");
	size_t countFiles = parseInstalledInt(in, "file count");
	_dependencies = parseInstalledList(in, countDependencies, "dependency");
	_provided_files = parseInstalledList(in, countFiles, "file");
}

void Package::parseInstalledV1(std::istream& in){
	FUN();

	_name = parseInstalledString(in, "name");
	_realVersion = parseInstalledInt(in, "realversion");
	_versionString = parseInstalledString(in, "version");
	_installed_md5 = parseInstalledString(in, "hash");
	size_t countDependencies = parseInstalledInt(in, "dependency count");
	size_t countFiles = parseInstalledInt(in, "file count");
	_dependencies = parseInstalledList(in, countDependencies, "dependency");
	_provided_files = parseInstalledList(in, countFiles, "file");
}
