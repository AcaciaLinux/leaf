/**
 * @file		leafdb/resolveDependers.cpp
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		The depender resolving part of LeafDB
 * @copyright 	Copyright (c) 2022
 */

#include "log.h"
#include "error.h"
#include "leafdebug.h"
#include "leafconfig.h"

#include "leafdb.h"

void LeafDB::resolveDependers(std::deque<Package*>& all_dependers){
	FUN();

	//Iterate over all dependers and resolve their dependers
	for (Package* curPackage : all_dependers){

		//Check for every package in the database if it is dependent on curPackage
		for (auto& package : _packages){
			if (package.second->dependsOn(curPackage)){
				
				//Check if the package is already in the list
				bool alreadyIn = false;
				for (Package* p : all_dependers){
					if (p == package.second){
						alreadyIn = true;
					}
				}

				//If not, add it to the list of dependers
				if (!alreadyIn)
					all_dependers.push_back(package.second);

			}
		}
	}
}

std::deque<Package*> LeafDB::resolveDependers(Package* package){
	FUN();
	_error.clear();
	std::deque<Package*> dependers;
	dependers.push_back(package);

	resolveDependers(dependers);

	return dependers;
}
