/**
 * @file		downloader/getSet.cpp
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		The file containing all the boring getter and setter stuff
 * @copyright 	Copyright (c) 2022
 */

#include "downloader.h"
#include "leafconfig.h"
#include "error.h"

std::string Downloader::getMD5(){
	return _md5.hexdigest();
}
