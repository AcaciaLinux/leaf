/**
 * @file		file/file.cpp
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		The core implementation of File
 * @copyright 	Copyright (c) 2022
 */

#include "log.h"
#include "file.h"

File::File(std::string name, std::string path, std::string provider){
	FUN();
	
	{//Strip the path to remove leading "/"
		std::string msg = "Stripping \"" + path + "\" to \"";

		while(path.length() > 0 && path[0] == '/')
			path = path.substr(1);

		LOGF(msg + path + "\"");
	}

	_name = name;
	_path = path;
	_provider = provider;

	LOGF("Created file \"" + _name + "\" (" + _path + ")");
}

