/**
 * @file		t_cleaf_init.cpp
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		Tests regarding the function cleaf_init()
 * @copyright	Copyright (c) 2022 Max Kofler and the AcaciaLinux developers
 */

#include "../t_cleaf.h"

#include <fstream>

extern bool _cleaf_initialized;

TEST(Cleaf, cleaf_init_debug){
    FUN();

    LEAF_DEBUG_SET_FAIL("cleaf_init()");

    //Save the Log instance, there is a new one created in cleaf_init
    Log::Log* log = hlog;

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

//Tests if cleaf_init() handles the case, where there is already a log module in existance
TEST(Cleaf, cleaf_init_existing_log){
    FUN();

    LEAF_DEBUG_SET_FAIL("cleaf_init::hlog_nullptr");

    try {

        //Prevent the first check in cleaf_init() from engaging
        _cleaf_initialized = false;

        cleaf_init(LOGLEVEL_A);

        FAIL() << "cleaf_init() does not handle an existing Log instance correctly";
    } catch (LeafError* e){
        CHECK_EC(Error::DEBUG_EXCEPTION, e);
    } catch (...){
        F_WRONGEXCEPTION("LeafError*");
    }
}

//This test checks if _cleaf_initialized gets set
TEST(Cleaf, cleaf_init_set_initialized){
    FUN();

    _cleaf_initialized = false;
    ASSERT_FALSE(_cleaf_initialized) << "_cleaf_initialized has not been set to false";

    //Save the Log instance, there is a new one created in cleaf_init
    Log::Log* log = hlog;

    cleaf_init(LOGLEVEL_U);

    //Restore the Log instance
    hlog = log;

    ASSERT_TRUE(_cleaf_initialized) << "_cleaf_initialized does not get set";
}
