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

//Checks if the check for the ownership of the Log module works
TEST(Cleaf, cleaf_finalize_owns_hlog){
    FUN();

    LEAF_DEBUG_SET_FAIL("cleaf_finalize::hlog_not_owned");

    try {

        //Set the parameters correctly
        _cleaf_initialized = true;
        _cleaf_owns_hlog = false;

        cleaf_finalize();

        FAIL() << "cleaf_finalize() does not check if cleaf owns the Log instance";
    } catch (LeafError* e){
        CHECK_EC(Error::DEBUG_EXCEPTION, e);
    } catch (...){
        F_WRONGEXCEPTION("LeafError*");
    }
}

//Checks if the check for a nullptr hlog is made (does not check for real)
TEST(Cleaf, cleaf_finalize_check_hlog_nullptr){
    FUN();

    LEAF_DEBUG_SET_FAIL("cleaf_finalize::hlog_nullptr");

    Log::Log* oldLog = hlog;

    try {
        LEAF_DEBUG_SET_FAIL("cleaf_finalize::pre_delete");

        //Prevent the first checks from engaging
        _cleaf_initialized = true;
        _cleaf_owns_hlog = true;

        //This should trigger the nullptr check
        hlog = nullptr;

        //Assert death by using the hlog instance
        ASSERT_EXIT(cleaf_finalize(), ::testing::KilledBySignal(SIGSEGV), ".*") << "cleaf_finalize() does not check if the hlog instance is a nullptr";

        //Restore the log instance
        hlog = oldLog;
    } catch (LeafError* e){
        //Restore the log instance
        hlog = oldLog;
        CHECK_EC(Error::DEBUG_EXCEPTION, e);
    } catch (...){
        //Restore the log instance
        hlog = oldLog;
        F_WRONGEXCEPTION("LeafError*");
    }
}

//Checks if the deletion is made normally
TEST(Cleaf, cleaf_finalize){
    FUN();

    //Assert that all the checks pass
    _cleaf_initialized = true;
    _cleaf_owns_hlog = true;

    //Backup the hlog instance
    Log::Log* oldLog = hlog;

    //Give hlog a new value
    hlog = (Log::Log*) new uint8_t;

    cleaf_finalize();

    //Backup the hlog result and restore old hlog
    Log::Log* hlogAfterDelete = hlog;
    hlog = oldLog;

    //Check if _cleaf_initialized has been unset
    ASSERT_FALSE(_cleaf_initialized) << "cleaf_finalize() does not unset _cleaf_initialized";

    //Check if the resulting hlog has been made safe (nullptr)
    ASSERT_EQ(nullptr, hlogAfterDelete);
}
