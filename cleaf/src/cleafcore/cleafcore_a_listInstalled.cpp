/**
 * @file        cleafcore/cleafcore_a_listInstalled.cpp
 * @author      Max Kofler (kofler.max.dev@gmail.com)
 * @brief       The implementation of the function cleafcore_a_listInstalled()
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

extern "C" {

    int8_t cleafcore_a_listInstalled(struct cleafcore * const core){
        CHECK_CLEAF_INIT_RET("cleafcore_a_listInstalled()", CLEAFCORE_NOTINIT);
        FUN();
        LEAF_DEBUG_EX("cleafcore_a_listInstalled()");

        //Check if the core is invalid
        if (core == NULL || core->core == NULL)
            return CLEAFCORE_NOCORE;

        //Clear the error
        cleafcore_clear_error(core);

        //Get the Leafcore pointer
        Leafcore* leafcore = (Leafcore*)core->core;

        //Get a reference to the leafconfig struct in the core
        leaf_config_t& leafconfig = leafcore->getConfig();

        leafconfig.action = ACTION_LISTINSTALLED;

        try {
            leafcore->parseInstalled();
            leafcore->a_listInstalled();
        } catch (LeafError* e){
            #ifdef DEBUG
            //When debugging, rethrow all LeafErrors
                throw e;
            #else
                core->error = e;
                return CLEAFCORE_LEAFERROR;
            #endif
        }

        return 0;
    }

}
