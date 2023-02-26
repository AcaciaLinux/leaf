/**
 * @file		leafcore/leafcore.cpp
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		The core implementation of Leafcore
 * @copyright 	Copyright (c) 2022
 */

#include "log.h"
#include "leafconfig.h"
#include "leafcore.h"
#include "globals.h"

#include <fstream>

bool proceed = true;

Leafcore::Leafcore(){
	FUN();

	_packageListDB = new LeafDB(this);
	_installedDB = new LeafDB(this);

	parseConfig();
}

Leafcore::Leafcore(leaf_config_t conf) : _config(conf){
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
