/**
 * @file		package/fetch.cpp
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		The code for fetching a package from  the server
 * @copyright 	Copyright (c) 2022
 */

#include "log.h"
#include "leafdebug.h"
#include "package.h"
#include "leafconfig.h"

bool Package::resolveDependentPackages(){
    FUN();
    LEAF_DEBUG_EX("Package::resolveDependentPackages()");

    throw new LeafError(Error::FEATURE_NOT_IMPLEMENTED);

    return true;
}
