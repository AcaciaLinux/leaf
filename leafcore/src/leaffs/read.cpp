/**
 * @file		
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		The implementation of 
 * @copyright	Copyright (c) 2022
 */
#include "log.h"
#include "error.h"
#include "leafdebug.h"
#include "leafconfig.h"

#include "leaffs.h"

void LeafFS::read(bool recursive){
	FUN();
	LEAF_DEBUG_EX("LeafFS::read()");

	//First check the filesystem to read
	check();

	getFiles("", _curDir, recursive);
}
