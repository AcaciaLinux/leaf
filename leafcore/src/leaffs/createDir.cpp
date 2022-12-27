/**
 * @file		leaffs/createDir.cpp
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		The implementation of LeafFS::create_directory() and LeafFS::create_directories()
 * @copyright	Copyright (c) 2022
 */
#include "log.h"
#include "error.h"
#include "leafdebug.h"

#include "leaffs.h"

//TODO: Tests
bool LeafFS::create_directory(const std::filesystem::path& path, bool thr){
	FUN();
	LEAF_DEBUG_EX("LeafFS::create_directory()");

	std::error_code ec;
	bool res = std::filesystem::create_directory(path, ec);

	if (ec)
		if (thr)
			throw new LeafError(Error::MKDIR, ec);
		else
			return false;

	return res;
}

//TODO: Tests
bool LeafFS::create_directories(const std::filesystem::path& path, bool thr){
	FUN();
	LEAF_DEBUG_EX("LeafFS::create_directories()");

	std::error_code ec;
	bool res = std::filesystem::create_directories(path, ec);

	if (ec)
		if (thr)
			throw new LeafError(Error::MKDIR, ec);
		else
			return false;

	return res;
}
