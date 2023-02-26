/**
 * @file		leaffs/is.cpp
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		The implementation of LeafFS::is()
 * @copyright	Copyright (c) 2022
 */
#include "log.h"
#include "error.h"
#include "leafdebug.h"

#include "leaffs.h"
#include <filesystem>

bool LeafFS::is(const std::filesystem::path& path, LeafFS_Type type, bool canThrow){
	FUN();
	LEAF_DEBUG_EX("LeafFS::is()");

	std::error_code ec;
	bool res;

	namespace fs = std::filesystem;

	switch(type){

	case LEAFFS_DIR:
		res = fs::is_directory(path, ec);
		break;

	case LEAFFS_BLOCK_FILE:
		res = fs::is_block_file(path, ec);
		break;

	case LEAFFS_CHAR_FILE:
		res = fs::is_character_file(path, ec);
		break;

	case LEAFFS_FILE:
		res = fs::is_regular_file(path, ec);
		break;

	case LEAFFS_SOCKET:
		res = fs::is_socket(path, ec);
		break;

	case LEAFFS_SYMLINK:
		res = fs::is_symlink(path, ec);
		break;

	case LEAFFS_FIFO:
		res = fs::is_fifo(path, ec);
		break;

	case LEAFFS_OTHER:
		res = fs::is_other(path, ec);
		break;

	case LEAFFS_EMPTY:
		res = fs::is_empty(path, ec);
		break;

	}

	if (ec){
		if (canThrow)
			throw new LeafError(Error::FS_CHECK_TYPE, path, ec);
		else
			return false;
	}

	return res;
}
