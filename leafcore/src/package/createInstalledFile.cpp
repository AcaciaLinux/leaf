/**
 * @file		package/createInstalledFile.cpp
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		The implementation of Package::createInstalledFile()
 * @copyright	Copyright (c) 2022
 */
#include "log.h"
#include "leafdebug.h"
#include "leafconfig.h"
#include "package.h"

void Package::createInstalledFile(std::ostream& out){
	FUN();

	LEAF_DEBUG_EX("Package::createInstalledFile()");

	if (!out.good())
		throw new LeafError(Error::BAD_OSTREAM, "create installed file for " + getFullName());

	out << "1" << std::endl; //Version 1
	out << _name << std::endl; //The name of the package
	out << _realVersion << std::endl; //The real version number
	out << _versionString << std::endl; //The version string
	out << _installed_md5 << std::endl; //The MD5 of the installed package
	out << std::to_string(_dependencies.size()) << std::endl;
	out << std::to_string(_provided_files.size() + _provided_directories.size()) << std::endl;

	for (std::string dep : _dependencies)
		out << dep << std::endl;

	for (std::string file : _provided_files)
		out << file << std::endl;

	for (auto rit = _provided_directories.rbegin(); rit != _provided_directories.rend(); ++rit){
		out << *rit << std::endl;
	}
}
