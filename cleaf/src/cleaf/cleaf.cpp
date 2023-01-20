/**
 * @file        cleaf/cleaf.cpp
 * @author      Max Kofler (kofler.max.dev@gmail.com)
 * @brief       The implementation of the cleaf.h header
 * @copyright   Copyright (c) 2022 Max Kofler and the AcaciaLinux developers
 */

#include "log.h"
#include "cleaf.h"
#include "cleaf_check.h"

#include "leafdebug.h"
#include "dist.h"

#include <iostream>
#include <sstream>
#include <cstring>

/**
 * @brief   If cleaf has been initialized
 */
bool _cleaf_initialized = false;

/**
 * @brief   If the cleaf instance owns the Log instance and if it should delete it
 */
bool _cleaf_owns_hlog = false;

/**
 * @brief   A stringstream for caching the leaf output to read it later
 */
std::stringstream _ss_cache;

extern "C" {

    uint32_t cleaf_get_leafcore_major(){
        FUN();
        LEAF_DEBUG_EX("cleaf_get_leafcore_major()");
        return LEAFCORE_V_MAJOR;
    }

    uint32_t cleaf_get_leafcore_minor(){
        FUN();
        LEAF_DEBUG_EX("cleaf_get_leafcore_minor()");
        return LEAFCORE_V_MINOR;
    }

    uint32_t cleaf_get_leafcore_patch(){
        FUN();
        LEAF_DEBUG_EX("cleaf_get_leafcore_patch()");
        return LEAFCORE_V_PATCH;
    }

    void cleaf_init(enum loglevel level){
        LEAF_DEBUG_EX("cleaf_init()");

        //Check if cleaf isn't already initialized
        if (_cleaf_initialized){
            LOGAPI("[cleaf] Cleaf is already initialized!");

            //A debugging checkpoint to check the above switch
            LEAF_DEBUG_EX("cleaf_init::check_initialized");

            return;
        }

        //Check if we already have a log instance
        if (hlog != nullptr){
            FUN();
            LOGUW("[cleaf] There seems to already be a Log instance, skipping setup!");
            _cleaf_initialized = true;
            _cleaf_owns_hlog = false;

            //A debugging checkpoint to check if the above switch worked
            LEAF_DEBUG_EX("cleaf_init::hlog_nullptr");

            return;
        }

        hlog = new Log::Log();
        _cleaf_owns_hlog = true;
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

    void cleaf_finalize(){
        LEAF_DEBUG_EX("cleaf_finalize()");

        if (!_cleaf_initialized){
            LOGW("[cleaf] cleaf_init() does not seem to have been called, nothing to do here");

            //A debugging checkpoint to check this behaviour
            LEAF_DEBUG_EX("cleaf_finalize::init_check");

            return;
        }

        if (!_cleaf_owns_hlog){
            LOGD("[cleaf] Cleaf does not own hlog instance, skipping removal");

            _cleaf_initialized = false;
            _cleaf_owns_hlog = false;

            //A debugging checkpoint to check this behaviour
            LEAF_DEBUG_EX("cleaf_finalize::hlog_not_owned");

            return;
        }

        if (hlog == nullptr){
            std::cerr <<"[cleaf] cleaf_init() seems to have been called but Log instance is absent? (This is a bug)" << std::endl;

            //A debugging checkpoint to check this behaviour
            LEAF_DEBUG_EX("cleaf_finalize::hlog_nullptr");

            return;
        }

        //Ability to break this function befor deleting
        LEAF_DEBUG_EX("cleaf_finalize::pre_delete");

        //Delete the instance
        delete hlog;

        //Make the pointer safe
        hlog = nullptr;

        //And make cleaf unininizialized
        _cleaf_initialized = false;
    }

    uint8_t cleaf_is_initialized(){
        FUN();
        return _cleaf_initialized;
    }

    uint8_t cleaf_owns_log(){
        FUN();
        return _cleaf_owns_hlog;
    }

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

    char* cleaf_get_log(){
        std::string cache = _ss_cache.str();

        char* res = new char[cache.size()+1];

        std::strcpy(res, cache.c_str());

        return res;
    }

    void cleaf_clear_log(){
        _ss_cache.str("");
    }
}

//This is a guard that checks if cleaf has been uninitialized once the program finishes
class CleafInitGuard{
public:
    ~CleafInitGuard(){
        if (_cleaf_initialized){
            if (hlog != nullptr){
                LOGW("[cleaf] Cleaf is still initialized on program shutdown! (This is a message for the developer)");
            }
        }
    }
};
static CleafInitGuard __cleaf_init_guard__;
