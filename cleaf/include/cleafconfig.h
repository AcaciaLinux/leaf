/**
 * @file        cleafconfig.h
 * @author      Max Kofler (kofler.max.dev@gmail.com)
 * @brief       A header containing functions to manipulate the leaf configuration
 * @copyright   Copyright (c) 2022 Max Kofler and the AcaciaLinux developers
 */

#ifndef __CLEAF_CONFIG_H__
#define __CLEAF_CONFIG_H__

#include "cleafcore.h"

#include <stdint.h>

enum cleaf_config_redownload{
    CLEAF_CONFIG_REDOWNLOAD_NONE = 0,
    CLEAF_CONFIG_REDOWNLOAD_SPECIFIED = 1,
    CLEAF_CONFIG_REDOWNLOAD_ALL = 2,

    COUNT_CLEAF_REDOWNLOAD
};

enum cleaf_bool_config{
    CLEAF_B_CONFIG_NOASK = 0,
    CLEAF_B_CONFIG_NOCLEAN = 1,
    CLEAF_B_CONFIG_NOPROGRESS = 2,
    CLEAF_B_CONFIG_FORCE = 3,
    CLEAF_B_CONFIG_FORCEOVERWRITE = 4,
    CLEAF_B_CONFIG_RUNPREINSTALL = 5,
    CLEAF_B_CONFIG_RUNPOSTINSTALL = 6,
    CLEAF_B_CONFIG_INSTALLDEPS = 7,
    CLEAF_B_CONFIG_CHECKREMOTEHASHUPGRADE = 8,

    COUNT_CLEAF_BOOL_CONFIG
};

enum cleaf_string_config{
    CLEAF_S_CONFIG_ROOTDIR = 0,
    CLEAF_S_CONFIG_PKGLISTURL = 1,
    CLEAF_S_CONFIG_CACHEDIR = 2,
    CLEAF_S_CONFIG_DOWNLOADDIR = 3,
    CLEAF_S_CONFIG_PACKAGESDIR = 4,
    CLEAF_S_CONFIG_CONFIGDIR = 5,
    CLEAF_S_CONFIG_INSTALLEDDIR = 6,
    CLEAF_S_CONFIG_HOOKSDIR = 7,
    CLEAF_S_CONFIG_PKGLISTPATH = 8,
    CLEAF_S_CONFIG_CHROOTCMD = 9,
    CLEAF_S_CONFIG_RUNSCRIPTSDIR = 10,

    COUNT_CLEAF_STRING_CONFIG
};

//Return codes for the cleafconfig functions
#define CLEAFCONFIG_OK 0                    //Everything ok
#define CLEAFCONFIG_NOTINIT UINT8_MAX       //cleaf is not initialized
#define CLEAFCONFIG_NOCORE UINT8_MAX-1      //The supplied leafcore instance is invalid (NULL)
#define CLEAFCONFIG_RO_CONF UINT8_MAX-2     //The configuration is Read Only
#define CLEAFCONFIG_INV_CONF UINT8_MAX-3    //The confiugration is invalid (unknown)

#define CLEAFCONFIG_B_FALSE 0               //False
#define CLEAFCONFIG_B_TRUE 1                //True

#ifdef __cplusplus
extern "C" {
#endif

    /**
     * @brief   Sets a boolean cleaf config
     * @param   core            The cleafcore instance to set the config in
     * @param   conf            The config to set
     * @param   value           The value to set (CLEAFCONFIG_B_FALSE/TRUE)
     * @return  uint8_t         CLEAFCONFIG_OK, else an error code defined above
     */
    uint8_t                     cleafconfig_setBoolConfig(struct cleafcore* core, enum cleaf_bool_config conf, uint8_t value);

    /**
     * @brief   Gets a boolean cleaf config
     * @param   core            The cleafcore instance to get the config from
     * @param   conf            The config to get
     * @return  uint8_t         CLEAFCONFIG_B_FALSE / CLEAFCONFIG_B_TRUE, else an error code defined above
     */
    uint8_t                     cleafconfig_getBoolConfig(struct cleafcore* core, enum cleaf_bool_config conf);

    /**
     * @brief   Sets a string cleaf config
     * @param   core            The cleafcore instance to set the config in
     * @param   conf            The config to set
     * @param   value           The value to set (does not take ownership)
     * @return  uint8_t         CLEAFCONFIG_OK, else an error code defined above
     */
    uint8_t                     cleafconfig_setStringConfig(struct cleafcore* core, enum cleaf_string_config config, const char* value);

    /**
     * @brief   Gets a string cleaf config
     * @param   core            The cleafcore instance to get the config from
     * @param   conf            The config to get
     * @return  const char*     A c string you have to delete or NULL on error
     */
    const char*                 cleafconfig_getStringConfig(struct cleafcore* core, enum cleaf_string_config config);

    /**
     * @brief   Sets the leafcore redownload behaviour
     * @param   core            The cleafcore instance to adjust
     * @param   mode            The mode to use
     * @return  uint8_t         CLEAFCONFIG_OK, else an error code defined above
     */
    uint8_t                     cleafconfig_setRedownload(struct cleafcore* core, enum cleaf_config_redownload mode);

#ifdef __cplusplus
}
#endif

#endif
