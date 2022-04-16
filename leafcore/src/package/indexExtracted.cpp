/**
 * @file		package/indexExtracted.cpp
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		The code for copying the extracted package to its destination
 * @copyright 	Copyright (c) 2022
 */

#include "log.h"
#include "leafdebug.h"
#include "package.h"
#include "leafconfig.h"
#include "leaffs.h"

#include <filesystem>

void Package::indexExtracted(){
	FUN();
	LEAF_DEBUG_EX("Package::indexExtracted()");

	LOGI("Indexing package " + getFullName() + " at " + getExtractedDir());

	if (!std::filesystem::exists(getExtractedDir()))
		throw new LeafError(Error::PKG_NOTEXTRACTED, getFullName());

	LeafFS fs(getExtractedDir() + "data/");

	fs.read(true);

	_provided_files = fs.getFiles();
	_provided_directories = fs.getDirectories();
}