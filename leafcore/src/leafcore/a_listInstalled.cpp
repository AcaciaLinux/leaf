/**
 * @file		leafcore/a_listInstalled
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		The implementation of Leafcore::a_listInstalled()
 * @copyright	Copyright (c) 2022
 */
#include "log.h"
#include "leafdebug.h"
#include "leafconfig.h"

#include "leafcore.h"

void Leafcore::a_listInstalled(){
	FUN();
	LEAF_DEBUG_EX("Leafcore::a_listInstalled()");

	checkDirectories();

	parseInstalled();

	for(auto pair : _installedDB->getPackages()){
		LOGU(pair.first + " (" + pair.second->getVersion() + ")");
	}
}
