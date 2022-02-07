/**
 * @file		file/getSet.cpp
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		The file containing all the boring getter and setter stuff
 * @copyright 	Copyright (c) 2022
 */

#include "file.h"

std::string File::getName(){
	return _name;
}

std::string File::getPath(){
	return _path;
}