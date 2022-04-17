/**
 * @file		leafdb/leafdb.cpp
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		The core implementation of LeafDB
 * @copyright 	Copyright (c) 2022
 */

#include "log.h"
#include "leafdb.h"

LeafDB::LeafDB(Leafcore* core){
	FUN();

	_core = core;
}

LeafDB::~LeafDB(){
	FUN();

	this->clear();
}

void LeafDB::clear(){
	FUN();

	for (auto entry : _packages){
		delete entry.second;
	}

	_packages.clear();
}
