/**
 * @file		package/copyToRoot.cpp
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		The code for copying the extracted package to its destination
 * @copyright 	Copyright (c) 2022
 */

#include "log.h"
#include "leafdebug.h"
#include "package.h"
#include "leafconfig.h"
#include "globals.h"

#include "leaffs.h"

#include <filesystem>

void Package::copyToRoot(const Leaf::config& conf, bool forceOverwrite){
    FUN();
    LEAF_DEBUG_EX("Package::copyToRoot()");

    std::string rootDir = conf.root;

    LOGI("[Package][copyToRoot] Copying package " + getFullName() + " to root " + rootDir);

    namespace fs = std::filesystem;

    if (!fs::exists(rootDir))
        throw new LeafError(Error::NOROOT, rootDir);

    if (!fs::exists(getExtractedDir(conf)))
        throw new LeafError(Error::PKG_NOTEXTRACTED, getFullName());

    indexExtracted(conf);

    //If leaf should not overwrite existing files, check for them
    if (!forceOverwrite){
        //Got through every entry
        for (std::string file : _provided_files){

            //If the entry exists and is not a directory, error out
            if (LeafFS::exists(rootDir + file) && !LeafFS::is(rootDir + file, LEAFFS_DIR))
                throw new LeafError(Error::PACKAGE_FILE_EXISTS, rootDir + file);
        }
    }

    //Finally, copy the package
    std::error_code ec;

    fs::copy_options options = fs::copy_options::copy_symlinks;

    std::string dataDir = getExtractedDir(conf).append("data");
    std::string destDir = rootDir;

    LOGI("[Package][copyToRoot] Creating destination directories...");

    //First, create the directories for the files
    for (std::string dir : _provided_directories){
        LOGF("[Package][copyToRoot] Creating directory " + destDir + dir);

        if (LeafFS::exists(destDir + dir) && LeafFS::is(destDir + dir, LEAFFS_SYMLINK)){
            LOGI("[Package][copyToRoot] Skipping creation of directory that is a symlink");
            continue;
        }

        LeafFS::create_directories(destDir + dir);
    }

    LOGI("[Package][copyToRoot] Copying files...");

    std::deque<std::string> copied_files;
    //Now copy all the files
    for (std::string file : _provided_files){
        if (!proceed){
            LOGUW("Copy for package " + getFullName() + " aborted, rolling back...");
            _provided_files = copied_files;

            try {
                removeFromRoot(conf);
            } catch (LeafError* e) {
                e->prepend("When rolling back copy for package " + getFullName() + ": ");
            }

            throw new LeafError(Error::ABORT);
        }

        std::string fPath = destDir + file;

        //If leaf should overwrite the files, delete the old files
        if (forceOverwrite){
            bool remove = 	LeafFS::is(fPath, LEAFFS_SYMLINK, false) ||	//If the entry is a symlink (even broken)
                            LeafFS::exists(fPath, false);				//or exists, remove it

            if (remove)
                LeafFS::remove_all(fPath);
        }

        LOGF("[Package][copyToRoot] Copying " + dataDir + file + " -> " + destDir + file);
        fs::copy(dataDir + file, destDir + file, options, ec);
        copied_files.push_back(file);

        if (ec)
            throw new LeafError(Error::COPYFILE, dataDir + file, ec);
    }
}
