/**
 * @file		package/clearcache.cpp
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		The implementation of Package::clearCache()
 * @copyright	Copyright (c) 2022
 */
#include "log.h"
#include "error.h"
#include "leafdebug.h"
#include "leafconfig.h"

#include "package.h"

#include <filesystem>

//TODO: Tests
void Package::clearCache(const Leaf::config& conf){
    FUN();
    LEAF_DEBUG_EX("Package::clearCache()");

    if (_isCollection){
        LOGI("[Package][clearCache] Skipping cache clearing of collection " + getFullName());
        return;
    }

    std::string extractedDir = getExtractedDir(conf);

    //If the cache directory exists, delete it
    if (std::filesystem::exists(extractedDir)){
        LOGI("[Package][clearCache] Deleting cache of package " + getFullName() + " at " + extractedDir + "...");
        std::error_code errCode;
        std::filesystem::remove_all(extractedDir, errCode);
        if (errCode){
            throw new LeafError(Error::REMOVEDIR, extractedDir, errCode);
        }
    } else {
        LOGI("[Package][clearCache] Cache directory " + extractedDir + " of package " + getFullName() + " did not exist");
    }
}
