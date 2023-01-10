/**
 * @file		t_cleaf_init.cpp
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		Tests regarding the function cleaf_init()
 * @copyright	Copyright (c) 2022 Max Kofler and the AcaciaLinux developers
 */

#include "../t_cleaf.h"

#include <fstream>

extern bool _cleaf_initialized;
extern bool _cleaf_owns_hlog;

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
        ASSERT_FALSE(_cleaf_owns_hlog) << "cleaf_init() claimed ownership of Log instance wrongly";
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

//Tests if cleaf claims the ownership of the Log instance correctly
TEST(Cleaf, cleaf_init_claim_hlog){
    FUN();

    //Store the old Log instance aside
    Log::Log* oldLog = hlog;
    hlog = nullptr;

    //Init cleaf to claim Log instance
    _cleaf_initialized = false;
    cleaf_init(LOGLEVEL_A);

    //Save the state for later checking
    bool owned = _cleaf_owns_hlog;

    //Finalize to clean up
    cleaf_finalize();

    //Restore Log instance
    hlog = oldLog;

    ASSERT_TRUE(owned) << "cleaf does not claim ownership of non exising Log module";
}

TEST(Cleaf, cleaf_is_initialized){
    FUN();

    _cleaf_initialized = true;
    ASSERT_TRUE(cleaf_is_initialized());

    _cleaf_initialized = false;
    ASSERT_FALSE(cleaf_is_initialized());
}

TEST(Cleaf, cleaf_owns_log){
    FUN();

    _cleaf_owns_hlog = true;
    ASSERT_TRUE(cleaf_owns_log());

    _cleaf_owns_hlog = false;
    ASSERT_FALSE(cleaf_owns_log());
}
