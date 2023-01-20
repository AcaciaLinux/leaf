/**
 * @file        cleafcore/cleafcore_a_installLocal.cpp
 * @author      Max Kofler (kofler.max.dev@gmail.com)
 * @brief       The implementation of the function cleafcore_a_installLocal()
 * @copyright   Copyright (c) 2022 Max Kofler and the AcaciaLinux developers
 */

#include "log.h"
#include "leafdebug.h"

#include "cleaf_check.h"
#include "cleaf.h"
#include "cleafcore.h"
#include "cleafconfig.h"

#include "leafcore.h"
#include "leafconfig.h"
#include "error.h"

int8_t cleafcore_a_installLocal(struct cleafcore* core, uint32_t len_pkgs, const char* pkgs[]){
    CHECK_CLEAF_INIT_RET("cleafcore_a_installLocal()", CLEAFCORE_NOTINIT);
    FUN();
    LEAF_DEBUG_EX("cleafcore_a_installLocal()");

    //Check if the core is invalid
    if (core == NULL || core->core == NULL)
        return CLEAFCORE_NOCORE;

    //Clear the error
    cleafcore_clear_error(core);

    //Get the Leafcore pointer
    Leafcore* leafcore = (Leafcore*)core->core;

    //Get a reference to the leafconfig struct in the core
    leaf_config_t& leafconfig = leafcore->getConfig();

    leafconfig.action = ACTION_UPDATE;

    try {
        leafcore->parseHooks();
        leafcore->parsePackageList();
        leafcore->parseInstalled();
        leafcore->a_installLocal(cleaf_pkgs_to_v(len_pkgs, pkgs));
    } catch (LeafError* e){

        #ifdef DEBUG
        //When debugging, rethrow DEBUG_EXCEPTION
        if (e->getErrorCode() == Error::DEBUG_EXCEPTION)
            throw e;
        #endif

        core->error = e;
        return CLAEFCORE_LEAFERROR;
    }

    return 0;
}
