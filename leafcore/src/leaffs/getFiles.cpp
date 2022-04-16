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

#include <filesystem>
namespace fs = std::filesystem;

void LeafFS::getFiles(std::string prefix, std::string directory, bool recursive){
	FUN();
	LEAF_DEBUG_EX("LeafFS::getFiles()");

	//TODO check for errors when indexing
	fs::directory_iterator dirIt(directory);

	std::string filePath;
	for (fs::directory_entry entry : dirIt){

		filePath = prefix + "/" + entry.path().stem().string() + entry.path().extension().string();

		LOGF("Indexing " + filePath);

		if (entry.is_symlink()){
			//Add the symlink
			_files.push_back(filePath);
		} else if (entry.is_directory()){
			
			//Add the directory
			_directories.push_back(filePath);

			//If recursive operation is nedded, do it
			if (recursive)
				getFiles(filePath, entry.path(), recursive);
		} else 
			_files.push_back(filePath);
	}
}
