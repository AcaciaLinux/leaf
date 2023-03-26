/**
 * @file        cleafcore/alloc.cpp
 * @author      Max Kofler (kofler.max.dev@gmail.com)
 * @brief       The implementation of allocation functions for cleafcore (new, delete)
 * @copyright   Copyright (c) 2022 Max Kofler and the AcaciaLinux developers
 */

#include "log.h"
#include "cleaf_check.h"

#include "leafdebug.h"
#include "leafcore.h"

#include "cleafcore.h"

extern "C" {

    struct cleafcore* cleafcore_new(){
		CHECK_CLEAF_INIT_RET("cleafcore_new()", NULL);
        FUN();
        LEAF_DEBUG_EX("cleafcore_new()");

        struct cleafcore* c = new struct cleafcore;
        c->core = new Leafcore();
        c->error = NULL;
        return c;
    }

    void cleafcore_delete(struct cleafcore* c){
		CHECK_CLEAF_INIT("cleafcore_delete()");
        FUN();
        LEAF_DEBUG_EX("cleafcore_delete()");

        //Remove the error object if present
        cleafcore_clear_error(c);

        delete (Leafcore*)c->core;
        delete c;
    }

}
