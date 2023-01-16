/**
 * @file        cleafcore/error.cpp
 * @author      Max Kofler (kofler.max.dev@gmail.com)
 * @brief       The implementation of error handling functions for cleafcore (get, set, clear...)
 * @copyright   Copyright (c) 2022 Max Kofler and the AcaciaLinux developers
 */

#include "log.h"

#include "leafdebug.h"
#include "error.h"
#include "leafcore.h"

#include "cleafcore.h"

#include <cstring>

extern "C" {

    uint16_t cleafcore_getError(struct cleafcore* c){
        FUN();
        LEAF_DEBUG_EX("cleafcore_getError()");

        if (c->error == NULL)
            return 0;
        else
            return ((LeafError*)c->error)->getErrorCode();
    }

    const char* cleafcore_getErrorString(struct cleafcore* c){
        FUN();
        LEAF_DEBUG_EX("cleafcore_getErrorString()");

        std::string what = "No error";

        if (c->error != NULL)
            what = ((LeafError*)c->error)->what();

        char* res = new char[what.size()+1];

        std::strcpy(res, what.c_str());

        return res;
    }

    void cleafcore_clear_error(struct cleafcore* c){
        FUN();
        LEAF_DEBUG_EX("cleafcore_clear_error()");

        if (c->error != NULL) {
            delete (LeafError*)c->error;
            c->error = NULL;
        }
    }

}
