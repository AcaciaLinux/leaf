/**
 * @file        config/config.cpp
 * @author      Max Kofler (kofler.max.dev@gmail.com)
 * @brief       The implementation of some functions in Leaf::config
 * @copyright   Copyright (c) 2023 Max Kofler and the AcaciaLinux developers
 */
#include "log.h"
#include "error.h"
#include "leafdebug.h"

#include "config.h"
#include "leaffs.h"

namespace Leaf {

    std::filesystem::path config::cacheDir(){
        if (extCacheDir.empty())
            return std::filesystem::path(root).append("var").append("cache").append("leaf");
        else
            return extCacheDir;
    }

    std::filesystem::path config::configDir(){
        if (extConfDir.empty())
            return std::filesystem::path(root).append("etc").append("leaf");
        else return extConfDir;
    }

    std::filesystem::path config::downloadDir(){
        if (extDLCache.empty())
            return cacheDir().append("downloads");
        else
            return extDLCache;
    }

    std::filesystem::path config::packagesDir(){
        if (extPKGCache.empty())
            return cacheDir().append("packages");
        else
            return extPKGCache;
    }

    std::filesystem::path config::runDir(){
        if (extRUNCache.empty())
            return cacheDir().append("run");
        else
            return extRUNCache;
    }

    std::filesystem::path config::installedDir(){
        if (extInstalledDir.empty())
            return configDir().append("installed");
        else
            return extInstalledDir;
    }

    std::filesystem::path config::hooksDir(){
        if (extHooksDir.empty())
            return configDir().append("hooks");
        else
            return extHooksDir;
    }

}