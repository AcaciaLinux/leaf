/**
 * @file		package/dependsOn.cpp
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		The implementation of Package::dependsOn()
 * @copyright	Copyright (c) 2022
 */
#include "log.h"
#include "error.h"
#include "leafdebug.h"

#include "package.h"

//TODO: Tests
bool Package::dependsOn(Package* p){
	FUN();
	LEAF_DEBUG_EX("Package::dependsOn()");

	bool depends = false;

	std::string p_name = p->getName();
	for (std::string depName : _dependencies){
		if (depName == p_name){
			depends = true;
			break;
		}
	}

	LOGD("[Package][dependsOn] " + getName() + " depends on " + p->getName() + ": " + (depends ? "YES" : "NO"));

	return depends;
}
