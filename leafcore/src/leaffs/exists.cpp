/**
 * @file		leaffs/exists.cpp
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		The implementation of LeafFS::exists()
 * @copyright	Copyright (c) 2022
 */
#include "log.h"
#include "error.h"
#include "leafdebug.h"

#include "leaffs.h"

//TODO: Tests
bool LeafFS::exists(const std::filesystem::path& path, bool thr){
	FUN();
	LEAF_DEBUG_EX("LeafFS::exists()");

	std::error_code ec;
	bool res = std::filesystem::exists(path, ec);

	if (ec)
		if (thr)
			throw new LeafError(Error::FS_EXISTS, ec);
		else
			return false;

	return res;
}
