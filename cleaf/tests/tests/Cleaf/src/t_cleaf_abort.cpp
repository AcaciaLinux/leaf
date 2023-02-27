/**
 * @file		t_cleaf_abort.cpp
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		Tests regarding the function cleaf_abort()
 * @copyright	Copyright (c) 2022 Max Kofler and the AcaciaLinux developers
 */

#include "../t_cleaf.h"
#include "globals.h"

#include <fstream>

TEST(Cleaf, cleaf_abort_debug){
    FUN();

    LEAF_DEBUG_SET_FAIL("cleaf_abort()");

    try{
        cleaf_abort();

        F_NOTHROW(Error::DEBUG_EXCEPTION);
    } catch (LeafError* e){
        CHECK_EC(Error::DEBUG_EXCEPTION, e);
    } catch (...){
        F_WRONGEXCEPTION("LeafError*");
    }
}

TEST(cleaf, cleaf_abort){
    FUN();

    proceed = true;
    cleaf_abort();
    ASSERT_FALSE(proceed) << "cleaf_abort() does not set proceed to false";
    cleaf_abort();
    ASSERT_FALSE(proceed) << "cleaf_abort() does seem to toggle proceed";
}
