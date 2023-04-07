/**
 * @file		package/removeFromRoot.cpp
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		The code for fetching a package from  the server
 * @copyright 	Copyright (c) 2022
 */

#include "log.h"
#include "leafdebug.h"
#include "package.h"
#include "leafconfig.h"

#include "leaffs.h"

#include <fstream>

void Package::removeFromRoot(const Leaf::config& conf){
    FUN();
    LEAF_DEBUG_EX("Package::removeFromRoot()");

    std::string rootDir = conf.root;

    for (std::string entry : _provided_files){
        if (LeafFS::is(rootDir + entry, LEAFFS_DIR)){
            if (LeafFS::is(rootDir + entry, LEAFFS_EMPTY)){
                LOGF("[Package][removeFromRoot] Removing empty directory " + rootDir + entry);
            } else {
                LOGF("[Package][removeFromRoot] Skipping non empty directory " + rootDir + entry);
                continue;
            }
        }

        LOGF("[Package][removeFromRoot] Removing entry " + rootDir + entry);
        LeafFS::remove_all(rootDir + entry);
    }

    //Finally remove the leafinstalled file
    LeafFS::remove(getInstalledFilePath(conf));
}
