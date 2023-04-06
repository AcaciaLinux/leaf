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

    std::filesystem::path config::cacheDir() const{
        if (extCacheDir.empty())
            return std::filesystem::path(root).append("var").append("cache").append("leaf");
        else
            return extCacheDir;
    }

    std::filesystem::path config::configDir() const{
        if (extConfDir.empty())
            return std::filesystem::path(root).append("etc").append("leaf");
        else return extConfDir;
    }

    std::filesystem::path config::downloadDir() const{
        if (extDLCache.empty())
            return cacheDir().append("downloads");
        else
            return extDLCache;
    }

    std::filesystem::path config::packagesDir() const{
        if (extPKGCache.empty())
            return cacheDir().append("packages");
        else
            return extPKGCache;
    }

    std::filesystem::path config::runDir() const{
        if (extRUNCache.empty())
            return cacheDir().append("run");
        else
            return extRUNCache;
    }

    std::filesystem::path config::installedDir() const{
        if (extInstalledDir.empty())
            return configDir().append("installed");
        else
            return extInstalledDir;
    }

    std::filesystem::path config::hooksDir() const{
        if (extHooksDir.empty())
            return configDir().append("hooks");
        else
            return extHooksDir;
    }

    std::filesystem::path config::mirrorsDir() const{
        if (extMirrorsDir.empty())
            return configDir().append("mirrors");
        else
            return extMirrorsDir;
    }

}
