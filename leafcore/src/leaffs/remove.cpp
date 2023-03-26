/**
 * @file		leaffs/remove.cpp
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		The implementation of LeafFS::remove() and LeafFS::remove_all()
 * @copyright	Copyright (c) 2022
 */
#include "log.h"
#include "error.h"
#include "leafdebug.h"

#include "leaffs.h"

//TODO: Tests
bool LeafFS::remove(const std::filesystem::path& path, bool thr){
	FUN();
	LEAF_DEBUG_EX("LeafFS::remove()");

	std::error_code ec;
	bool res = std::filesystem::remove(path, ec);

	if (ec)
		if (thr)
			throw new LeafError(Error::REMOVE, ec);
		else
			return false;

	return res;
}

//TODO: Tests
bool LeafFS::remove_all(const std::filesystem::path& path, bool thr){
	FUN();
	LEAF_DEBUG_EX("LeafFS::remove_all()");

	std::error_code ec;
	bool res = std::filesystem::remove_all(path, ec);

	if (ec)
		if (thr)
			throw new LeafError(Error::REMOVE, ec);
		else
			return false;

	return res;
}
