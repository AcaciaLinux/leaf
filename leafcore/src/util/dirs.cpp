/**
 * @file        util/dirs
 * @author      Max Kofler (kofler.max.dev@gmail.com)
 * @brief       The implementation of ensureDirs()
 * @copyright   Copyright (c) 2023 Max Kofler and the AcaciaLinux developers
 */
#include "log.h"
#include "error.h"
#include "leafdebug.h"

#include "util.h"
#include "leaffs.h"

//TODO: Tests
void LeafUtil::ensureDirs(const Leaf::config& conf){
    FUN();
    LEAF_DEBUG_EX("LeafUtil::ensureDirs()");

    {//First, create the parent configuration directory
        if (!LeafFS::exists(conf.configDir())){
            if (LeafUtil::askUserOK(conf, "Configuration directory " + std::string(conf.configDir()) + " does not exist, create it?", true))
                    LeafFS::create_directories(conf.configDir());
            else
                    throw new LeafError(Error::USER_DISAGREE, "Creating config directory " + std::string(conf.configDir()));
        }
        if (!LeafFS::is(conf.configDir(), LEAFFS_DIR))
            throw new LeafError(Error::NOTDIR, "Configuration directory " + std::string(conf.configDir()));

        {//And then all the children
            if (!LeafFS::exists(conf.installedDir()))
                LeafFS::create_directories(conf.installedDir());

            if (!LeafFS::exists(conf.hooksDir()))
                LeafFS::create_directories(conf.hooksDir());

            if (!LeafFS::exists(conf.mirrorsDir()))
                LeafFS::create_directories(conf.mirrorsDir());
        }
    }

    {//First, create the parent cache directory
        if (!LeafFS::exists(conf.cacheDir())){
            if (LeafUtil::askUserOK(conf, "Cache directory " + std::string(conf.cacheDir()) + " does not exist, create it?", true))
                    LeafFS::create_directories(conf.cacheDir());
            else
                    throw new LeafError(Error::USER_DISAGREE, "Creating cache directory " + std::string(conf.cacheDir()));
        }
        if (!LeafFS::is(conf.cacheDir(), LEAFFS_DIR))
            throw new LeafError(Error::NOTDIR, "Cache directory " + std::string(conf.cacheDir()));

        {//And then all the children
            if (!LeafFS::exists(conf.downloadDir()))
                LeafFS::create_directories(conf.downloadDir());

            if (!LeafFS::exists(conf.packagesDir()))
                LeafFS::create_directories(conf.packagesDir());

            if (!LeafFS::exists(conf.runDir()))
                LeafFS::create_directories(conf.runDir());
        }
    }

}
