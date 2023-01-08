/**
 * @file        cleaf/cleaf.cpp
 * @author      Max Kofler (kofler.max.dev@gmail.com)
 * @brief       The implementation of the cleaf.h header
 * @copyright   Copyright (c) 2022 Max Kofler and the AcaciaLinux developers
 */

#include "log.h"
#include "cleaf.h"

#include "leafdebug.h"
#include "dist.h"

#include <iostream>
#include <sstream>

/**
 * @brief   If cleaf has been initialized
 */
bool _cleaf_initialized = false;

/**
 * @brief   A stringstream for caching the leaf output to read it later
 */
std::stringstream _ss_cache;

#define CHECK_CLEAF_INIT(fn) { \
    if (!_cleaf_initialized){ \
        std::cerr << "[cleaf] cleaf is not initialized! Unable to perform " << fn << std::endl; \
        return; \
    } \
} \

extern "C" {

    unsigned int cleaf_get_leafcore_major(){
        FUN();
        LEAF_DEBUG_EX("cleaf_get_leafcore_major()");
        return LEAFCORE_V_MAJOR;
    }

    unsigned int cleaf_get_leafcore_minor(){
        FUN();
        LEAF_DEBUG_EX("cleaf_get_leafcore_minor()");
        return LEAFCORE_V_MINOR;
    }

    unsigned int cleaf_get_leafcore_patch(){
        FUN();
        LEAF_DEBUG_EX("cleaf_get_leafcore_patch()");
        return LEAFCORE_V_PATCH;
    }

    void cleaf_init(enum loglevel level){
        LEAF_DEBUG_EX("cleaf_init()");
        hlog = new Log::Log();
        FUN();

        {//Setup the stdout stream
            Log::stream_config c;
            c.loglevel = (Log::level) level;
            c.print_function_names = false;
            hlog->addStream(std::cout, c);
        }

        {//Setup the cached stream
            Log::stream_config c;
            c.loglevel = (Log::level) level;
            c.print_function_names = false;
            hlog->addStream(_ss_cache, c);
        }

        _cleaf_initialized = true;
    }

    #warning Tests for loglevel, add dummy cleaf_init() function that doesn't mess with the hlog instance
    void cleaf_setLogLevel(enum loglevel level){
        LEAF_DEBUG_EX("cleaf_setLogLevel()");

        //Check if cleaf is initialized
        CHECK_CLEAF_INIT("cleaf_setLogLevel()");

        LEAF_DEBUG_EX("cleaf_setLogLevel::afterInitCheck");
        FUN();

        {//Change the stdout stream
            Log::stream_config* c = hlog->getStreamConf(std::cout);
            if (!c){
                LOGUE("[cleaf] Unable to get stdout stream config! (This is a bug)");
                return;
            }

            c->loglevel = (Log::level) level;
        }

        {//Change the cached stream
            Log::stream_config* c = hlog->getStreamConf(_ss_cache);
            if (!c){
                LOGUE("[cleaf] Unable to get cached stream config! (This is a bug)");
                return;
            }

            c->loglevel = (Log::level) level;
        }
    }

    const char* cleaf_get_log(){
        return _ss_cache.str().c_str();
    }

    void cleaf_cleaf_log(){
        _ss_cache.clear();
    }
}
