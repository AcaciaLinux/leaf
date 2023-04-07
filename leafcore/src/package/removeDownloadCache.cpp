/**
 * @file		package/removeDownloadCache.cpp
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		The implementation of Package::removeDownloadCache()
 * @copyright	Copyright (c) 2022
 */
#include "log.h"
#include "error.h"
#include "leafdebug.h"
#include "leafconfig.h"

#include "package.h"
#include "leaffs.h"

//TODO: Tests
void Package::removeDownloadCache(const Leaf::config& conf){
    FUN();
    LEAF_DEBUG_EX("Package::removeDownloadCache()");

    //Check the existance of the file and omit the removal if not
    bool exists = LeafFS::exists(getDownloadPath(conf));

    if (!exists){
        LOGD("[Package][removeDownloadCache] Download cache file " + std::string(getDownloadPath(conf)) + " does not exist, skipping removal");
        return;
    }

    LeafFS::remove(getDownloadPath(conf));

    LOGI("[Package][removeDownloadCache] Removed download cache of package " + getFullName() + " at " + std::string(getDownloadPath(conf)));
}
