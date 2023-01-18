/**
 * @file        cleafconfig/cleafconfig_redownload.cpp
 * @author      Max Kofler (kofler.max.dev@gmail.com)
 * @brief       The implementation for the redownload config
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

static_assert(COUNT_CLEAF_REDOWNLOAD == 3, "Amount of CLEAF_STRING cofigs changed");
static_assert((int)COUNT_CONFIG_REDOWNLOAD == (int)COUNT_CLEAF_REDOWNLOAD, "CLEAF and LEAF redownload options do not match");

uint8_t cleafconfig_setRedownload(struct cleafcore* core, cleaf_config_redownload mode){
    CHECK_CLEAF_INIT_RET("cleafconfig_setRedownload()", CLEAFCONFIG_NOTINIT);
    FUN();
    LEAF_DEBUG_EX("cleafconfig_setRedownload()");

    //Check if the core is invalid
    if (core == NULL || core->core == NULL)
        return CLEAFCONFIG_NOCORE;

    //Get a reference to the leafconfig struct in the core
    leaf_config_t& leafconfig = ((Leafcore*)core->core)->getConfig();

    //Set the config
    leafconfig.redownload = (config_redownload) mode;

    return CLEAFCONFIG_OK;
}
