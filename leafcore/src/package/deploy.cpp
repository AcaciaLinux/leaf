/**
 * @file		package/deploy.cpp
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		The code for fetching a package from  the server
 * @copyright 	Copyright (c) 2022
 */

#include "log.h"
#include "leafdebug.h"
#include "error.h"
#include "package.h"
#include "leafconfig.h"

#include "util.h"
#include "leaffs.h"

#include <fstream>
#include <filesystem>

void Package::deploy(Leaf::conf_tr& conf){
    FUN();
    LEAF_DEBUG_EX("Package::deploy()");

    if (_isCollection){
        LOGI("[Package][deploy] Skipping deployment of collection " + getFullName());
        return;
    }

    LeafUtil::ensureDirs(conf.parent);

    bool overwrite = conf.force;
    if (LeafFS::exists(getInstalledFilePath(conf.parent))){
        LOGI("[Package][deploy] Leafinstalled file for package " + getName() + " exists, reinstalling...");
        overwrite = true;
    }

    try {

        runPreinstall(conf.parent);
        copyToRoot(conf.parent, overwrite);
        runPostinstall(conf.parent);

    } catch (LeafError* e) {
        LeafFS::remove(getInstalledFilePath(conf.parent));
        throw e;
    }

    {//Create the .leafinstalled file
        std::ofstream installedFile;
        installedFile.open(getInstalledFilePath(conf.parent), std::ios::trunc);

        if (!installedFile.is_open())
            throw new LeafError(Error::OPENFILEW, "Leafinstalled file " + std::string(getInstalledFilePath(conf.parent)) + " for " + getFullName());

        createInstalledFile(installedFile);

        installedFile.close();
    }
} 
