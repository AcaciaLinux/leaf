/**
 * @file		t_cleaf_finalize.cpp
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		Tests regarding the function cleaf_finalize()
 * @copyright	Copyright (c) 2022 Max Kofler and the AcaciaLinux developers
 */

#include "../t_cleaf.h"

#include <fstream>

extern bool _cleaf_initialized;
extern bool _cleaf_owns_hlog;

TEST(Cleaf, cleaf_finalize_debug){
    FUN();

    LEAF_DEBUG_SET_FAIL("cleaf_finalize()");

    //Save the Log instance, there is a new one created in cleaf_init
    Log::Log* log = hlog;
    hlog = nullptr;

    try{
        cleaf_init(LOGLEVEL_U);

        //And finalize cleaf to not leak any memory
        cleaf_finalize();

        //Restore the Log instance
        hlog = log;

        F_NOTHROW(Error::DEBUG_EXCEPTION);
    } catch (LeafError* e){
        //Restore the Log instance
        hlog = log;

        CHECK_EC(Error::DEBUG_EXCEPTION, e);
    } catch (...){
        //Restore the Log instance
        hlog = log;

        F_WRONGEXCEPTION("LeafError*");
    }
}

//Checks if the check for initialized cleaf works
TEST(Cleaf, cleaf_finalize_check_initialized){
    FUN();

    LEAF_DEBUG_SET_FAIL("cleaf_finalize::init_check");

    try {

        _cleaf_initialized = false;

        cleaf_finalize();

        FAIL() << "cleaf_finalize() does not check if cleaf has even been initialized";
    } catch (LeafError* e){
        CHECK_EC(Error::DEBUG_EXCEPTION, e);
    } catch (...){
        F_WRONGEXCEPTION("LeafError*");
    }
}
