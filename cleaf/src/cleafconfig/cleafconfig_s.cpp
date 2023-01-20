/**
 * @file        cleafconfig/cleafconfig_s.cpp
 * @author      Max Kofler (kofler.max.dev@gmail.com)
 * @brief       The implementation for the string cleaf configs
 * @copyright   Copyright (c) 2022 Max Kofler and the AcaciaLinux developers
 */

#include "log.h"
#include "leafdebug.h"

#include "cleafconfig.h"

#include "leafcore.h"
#include "leafconfig.h"
#include "cleaf_check.h"

#include <assert.h>
#include <cstring>

static_assert(COUNT_CLEAF_STRING_CONFIG == 11, "Amount of CLEAF_STRING cofigs changed");

uint8_t cleafconfig_setStringConfig(struct cleafcore* core, cleaf_string_config config, const char* val_c){
    CHECK_CLEAF_INIT_RET("cleafconfig_setStringConfig()", CLEAFCONFIG_NOTINIT);
    FUN();
    LEAF_DEBUG_EX("cleafconfig_setStringConfig()");

    //Check if the core is invalid
    if (core == NULL || core->core == NULL)
        return CLEAFCONFIG_NOCORE;

    //Get a reference to the leafconfig struct in the core
    leaf_config_t& leafconfig = ((Leafcore*)core->core)->getConfig();

    //Create a c++ string of the value
    std::string value = std::string(val_c);

    switch (config) {

        case CLEAF_S_CONFIG_ROOTDIR:
            LOGAPI("[cleafconfig] Setting CLEAF_S_CONFIG_ROOTDIR to '" + value + "'");
            leafconfig.setRootDir(value);
            break;

        case CLEAF_S_CONFIG_PKGLISTURL:
            LOGAPI("[cleafconfig] Setting CLEAF_S_CONFIG_PKGLISTURL to '" + value + "'");
            leafconfig.pkgListURL = value;
            break;

        case CLEAF_S_CONFIG_CHROOTCMD:
            LOGAPI("[cleafconfig] Setting CLEAF_S_CONFIG_CHROOTCMD to '" + value + "'");
            leafconfig.chroot_cmd = value;
            break;

        case CLEAF_S_CONFIG_CACHEDIR:
        case CLEAF_S_CONFIG_DOWNLOADDIR:
        case CLEAF_S_CONFIG_PACKAGESDIR:
        case CLEAF_S_CONFIG_CONFIGDIR:
        case CLEAF_S_CONFIG_INSTALLEDDIR:
        case CLEAF_S_CONFIG_HOOKSDIR:
        case CLEAF_S_CONFIG_PKGLISTPATH:
        case CLEAF_S_CONFIG_RUNSCRIPTSDIR:
            LOGAPI("[cleafconfig] The config " + std::to_string(config) + " is read-only");
            return CLEAFCONFIG_RO_CONF;

        default:
            return CLEAFCONFIG_INV_CONF;

    }

    return CLEAFCONFIG_OK;
}

const char* cleafconfig_getStringConfig(struct cleafcore* core, cleaf_string_config config){
    CHECK_CLEAF_INIT_RET("cleafconfig_getBoolConfig()", NULL);
    FUN();
    LEAF_DEBUG_EX("cleafconfig_getStringConfig()");

    //Check if the core is invalid
    if (core == NULL || core->core == NULL)
        return NULL;

    //Get a reference to the leafconfig struct in the core
    leaf_config_t leafconfig = ((Leafcore*)core->core)->getConfig();

    //The result
    std::string res = "Unknown";

    switch (config) {

        case CLEAF_S_CONFIG_ROOTDIR:
            res = leafconfig.rootDir;
            LOGAPI("[cleafconfig] Getting CLEAF_S_CONFIG_ROOTDIR: " + res);
            break;

        case CLEAF_S_CONFIG_PKGLISTURL:
            res = leafconfig.pkgListURL;
            LOGAPI("[cleafconfig] Getting CLEAF_S_CONFIG_PKGLISTURL: " + res);
            break;

        case CLEAF_S_CONFIG_CACHEDIR:
            res = leafconfig.cacheDir();
            LOGAPI("[cleafconfig] Getting CLEAF_S_CONFIG_CACHEDIR: " + res);
            break;

        case CLEAF_S_CONFIG_DOWNLOADDIR:
            res = leafconfig.downloadDir();
            LOGAPI("[cleafconfig] Getting CLEAF_S_CONFIG_DOWNLOADDIR: " + res);
            break;

        case CLEAF_S_CONFIG_PACKAGESDIR:
            res = leafconfig.packagesDir();
            LOGAPI("[cleafconfig] Getting CLEAF_S_CONFIG_PACKAGESDIR: " + res);
            break;

        case CLEAF_S_CONFIG_CONFIGDIR:
            res = leafconfig.configDir();
            LOGAPI("[cleafconfig] Getting CLEAF_S_CONFIG_CONFIGDIR: " + res);
            break;

        case CLEAF_S_CONFIG_INSTALLEDDIR:
            res = leafconfig.installedDir();
            LOGAPI("[cleafconfig] Getting CLEAF_S_CONFIG_INSTALLEDDIR: " + res);
            break;

        case CLEAF_S_CONFIG_HOOKSDIR:
            res = leafconfig.hooksDir();
            LOGAPI("[cleafconfig] Getting CLEAF_S_CONFIG_HOOKSDIR: " + res);
            break;

        case CLEAF_S_CONFIG_PKGLISTPATH:
            res = leafconfig.pkgListPath();
            LOGAPI("[cleafconfig] Getting CLEAF_S_CONFIG_PKGLISTPATH: " + res);
            break;

        case CLEAF_S_CONFIG_CHROOTCMD:
            res = leafconfig.chroot_cmd;
            LOGAPI("[cleafconfig] Getting CLEAF_S_CONFIG_CROOTCMD: " + res);
            break;

        case CLEAF_S_CONFIG_RUNSCRIPTSDIR:
            res = leafconfig.runScriptsDir();
            LOGAPI("[cleafconfig] Getting CLEAF_S_CONFIG_RUNSCRIPTSDIR: " + res);
            break;

        default:
            LOGAPI("[cleafconfig] Config " + std::to_string(config) + " is unknown");
            return NULL;

    }

    //Convert the string to a c string
    char* res_c = new char[res.size()+1];
    std::strcpy(res_c, res.c_str());

    return res_c;
}
