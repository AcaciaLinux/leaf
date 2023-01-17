/**
 * @file        cleafconfig/cleafconfig_b.cpp
 * @author      Max Kofler (kofler.max.dev@gmail.com)
 * @brief       The implementation for the boolean cleaf configs
 * @copyright   Copyright (c) 2022 Max Kofler and the AcaciaLinux developers
 */

#include "log.h"

#include "cleafconfig.h"

#include "leafcore.h"
#include "leafconfig.h"
#include "cleaf_check.h"

#include <assert.h>

static_assert(COUNT_CLEAF_BOOL_CONFIG == 9, "Amount of CLEAF_BOOL cofigs changed");

uint8_t cleafconfig_setBoolConfig(struct cleafcore* core, cleaf_bool_config config, uint8_t value){
    CHECK_CLEAF_INIT_RET("cleafconfig_setBoolConfig()", CLEAFCONFIG_NOTINIT);
    FUN();

    //Check if the core is invalid
    if (core == NULL || core->core == NULL)
        return CLEAFCONFIG_NOCORE;

    //Get a reference to the leafconfig struct in the core
    leaf_config_t leafconfig = ((Leafcore*)core->core)->getConfig();

    //Check if the value is bigger than 0 => true
    bool state = value > 0;

    //Switch the configs
    switch (config) {

    case CLEAF_B_CONFIG_NOASK:
        LOGAPI("[cleafconfig] Setting CLEAF_B_CONFIG_NOASK to " + std::to_string(state));
        leafconfig.noAsk = state;
        break;

    case CLEAF_B_CONFIG_NOCLEAN:
        LOGAPI("[cleafconfig] Setting CLEAF_B_CONFIG_NOCLEAN to " + std::to_string(state));
        leafconfig.noClean = state;
        break;

    case CLEAF_B_CONFIG_NOPROGRESS:
        LOGAPI("[cleafconfig] Setting CLEAF_B_CONFIG_NOPROGRESS to " + std::to_string(state));
        leafconfig.noProgress = state;
        break;

    case CLEAF_B_CONFIG_FORCE:
        LOGAPI("[cleafconfig] Setting CLEAF_B_CONFIG_FORCE to " + std::to_string(state));
        leafconfig.force = state;
        break;

    case CLEAF_B_CONFIG_FORCEOVERWRITE:
        LOGAPI("[cleafconfig] Setting CLEAF_B_CONFIG_FORCEOVERWRITE to " + std::to_string(state));
        leafconfig.forceOverwrite = state;
        break;

    case CLEAF_B_CONFIG_RUNPREINSTALL:
        LOGAPI("[cleafconfig] Setting CLEAF_B_CONFIG_RUNPREINSTALL to " + std::to_string(state));
        leafconfig.runPreinstall = state;
        break;

    case CLEAF_B_CONFIG_RUNPOSTINSTALL:
        LOGAPI("[cleafconfig] Setting CLEAF_B_CONFIG_RUNPOSTINSTALL to " + std::to_string(state));
        leafconfig.runPostinstall = state;
        break;

    case CLEAF_B_CONFIG_INSTALLDEPS:
        LOGAPI("[cleafconfig] Setting CLEAF_B_CONFIG_INSTALLDEPS to " + std::to_string(state));
        leafconfig.installDependencies = state;
        break;

    case CLEAF_B_CONFIG_CHECKREMOTEHASHUPGRADE:
        LOGAPI("[cleafconfig] Setting CLEAF_B_CONFIG_CHECKREMOTEHASHUPGRADEto " + std::to_string(state));
        leafconfig.checkRemoteHashUpgrade = state;
        break;

    default:
        return CLEAFCONFIG_INV_CONF;
    }

    return CLEAFCONFIG_OK;
}

uint8_t cleafconfig_getBoolConfig(struct cleafcore* core, cleaf_bool_config config){
    CHECK_CLEAF_INIT_RET("cleafconfig_getBoolConfig()", CLEAFCONFIG_NOTINIT);
    FUN();

    //Check if the core is invalid
    if (core == NULL || core->core == NULL)
        return CLEAFCONFIG_NOCORE;

    //Get a reference to the leafconfig struct in the core
    leaf_config_t leafconfig = ((Leafcore*)core->core)->getConfig();

    //The result
    uint8_t res = CLEAFCONFIG_B_FALSE;

    //Switch the configs
    switch (config) {

    case CLEAF_B_CONFIG_NOASK:
        res = leafconfig.noAsk;
        LOGAPI("[cleafconfig] Getting CLEAF_B_CONFIG_NOASK: " + std::to_string(res));
        break;

    case CLEAF_B_CONFIG_NOCLEAN:
        res = leafconfig.noClean;
        LOGAPI("[cleafconfig] Getting CLEAF_B_CONFIG_NOCLEAN: " + std::to_string(res));
        break;

    case CLEAF_B_CONFIG_NOPROGRESS:
        res = leafconfig.noProgress;
        LOGAPI("[cleafconfig] Getting CLEAF_B_CONFIG_NOPROGRESS: " + std::to_string(res));
        break;

    case CLEAF_B_CONFIG_FORCE:
        res = leafconfig.force;
        LOGAPI("[cleafconfig] Getting CLEAF_B_CONFIG_FORCE: " + std::to_string(res));
        break;

    case CLEAF_B_CONFIG_FORCEOVERWRITE:
        res = leafconfig.forceOverwrite;
        LOGAPI("[cleafconfig] Getting CLEAF_B_CONFIG_FORCEOVERWRITE: " + std::to_string(res));
        break;

    case CLEAF_B_CONFIG_RUNPREINSTALL:
        res = leafconfig.runPreinstall;
        LOGAPI("[cleafconfig] Getting CLEAF_B_CONFIG_RUNPREINSTALL: " + std::to_string(res));
        break;

    case CLEAF_B_CONFIG_RUNPOSTINSTALL:
        res = leafconfig.runPostinstall;
        LOGAPI("[cleafconfig] Getting CLEAF_B_CONFIG_RUNPOSTINSTALL: " + std::to_string(res));
        break;

    case CLEAF_B_CONFIG_INSTALLDEPS:
        res = leafconfig.installDependencies;
        LOGAPI("[cleafconfig] Getting CLEAF_B_CONFIG_INSTALLDEPS: " + std::to_string(res));
        break;

    case CLEAF_B_CONFIG_CHECKREMOTEHASHUPGRADE:
        res = leafconfig.checkRemoteHashUpgrade;
        LOGAPI("[cleafconfig] Getting CLEAF_B_CONFIG_CHECKREMOTEHASHUPGRADE: " + std::to_string(res));
        break;

    default:
        res = CLEAFCONFIG_INV_CONF;
        break;

    }

    return res;
}
