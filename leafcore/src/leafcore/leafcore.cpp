/**
 * @file		leafcore/leafcore.cpp
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		The core implementation of Leafcore
 * @copyright 	Copyright (c) 2022
 */

#include "log.h"
#include "leafconfig.h"
#include "leafcore.h"

#include <fstream>

Leafcore::Leafcore(){
	FUN();

	_packageListDB = new LeafDB(this);
	_installedDB = new LeafDB(this);

	parseConfig();
}

Leafcore::~Leafcore(){
	FUN();

	delete _packageListDB;
	delete _installedDB;
}
