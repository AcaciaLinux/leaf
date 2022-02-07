#include "log.h"
#include "package.h"

#include <algorithm>

Package::Package(std::string name){
	FUN();
	_name = name;
	LOGF("Created package \"" + name + "\"");
}

std::string Package::getName(){
	FUN();
	return _name;
}

std::string Package::getDescription(){
	FUN();
	return _description;
}

void Package::addProvidedFile(std::string filepath){
	FUN();
	{//Strip the path to remove leading "/"
		std::string msg = "Stripping \"" + filepath + "\" to \"";

		while(filepath.length() > 0 && filepath[0] == '/')
			filepath = filepath.substr(1);

		LOGF(msg + filepath + "\"");
	}

	_provided_files.push_back(filepath);
}

std::vector<std::string>& Package::getProvidedFiles(){
	return _provided_files;
}

std::vector<std::string>& Package::getDependencies(){
	return _dependencies;
}

bool Package::checkFileProvided(std::string filepath){
	FUN();

	{//Strip the path to remove leading "/"
		std::string msg = "Stripping \"" + filepath + "\" to \"";

		while(filepath.length() > 0 && filepath[0] == '/')
			filepath = filepath.substr(1);

		LOGF(msg + filepath + "\"");
	}

	std::vector<std::string>::iterator results = std::find(_provided_files.begin(), _provided_files.end(), filepath);
	return _provided_files.end() != results;
}

std::string Package::toString(){
	std::string buf = "Package ";
	buf += _name + " (" + _description + ")";
	for (std::string dep : _dependencies)
		buf += " [" + dep + "]";
	buf += " " + _fetchURL;

	return buf;
}