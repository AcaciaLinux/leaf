/**
 * @file		t_cleaf_alloc.cpp
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		Tests regarding the functions to handle Leafcore allocation an deallocation (new, delete)
 * @copyright	Copyright (c) 2022 Max Kofler and the AcaciaLinux developers
 */

#include "../t_cleafcore.h"

#include "cleafdebug.h"

//
// cleafcore_new()
//

TEST(Cleafcore, cleafcore_new_debug){
    FUN();
    CLEAF_INIT_DUMMY(true);

    LEAF_DEBUG_SET_FAIL("cleafcore_new()");

    try{
        struct cleafcore* c = cleafcore_new();

        F_NOTHROW(Error::DEBUG_EXCEPTION);
    } catch (LeafError* e){
        CHECK_EC(Error::DEBUG_EXCEPTION, e);
    } catch (...){
        F_WRONGEXCEPTION("LeafError*");
    }
}

TEST(Cleafcore, cleafcore_new_notInit){
    FUN();
    CLEAF_INIT_DUMMY(false);

    ASSERT_EQ(NULL, cleafcore_new()) << "cleafcore_new() does not check for initialized cleaf";
}

//Checks if the error pointer is null
TEST(Cleafcore, cleafcore_new_error_null){
    FUN();
    CLEAF_INIT_DUMMY(true);

    struct cleafcore* c = cleafcore_new();

    ASSERT_EQ(NULL, c->error) << "cleafcore_new() does not set the error pointer to NULL";

    cleafcore_delete(c);
}

//
// cleafcore_delete()
//

TEST(Cleafcore, cleafcore_delete_debug){
    FUN();
    CLEAF_INIT_DUMMY(true);

    LEAF_DEBUG_SET_FAIL("cleafcore_delete()");

    struct cleafcore* c = cleafcore_new();

    try{
        cleafcore_delete(c);

        F_NOTHROW(Error::DEBUG_EXCEPTION);
    } catch (LeafError* e){
        CHECK_EC(Error::DEBUG_EXCEPTION, e);
    } catch (...){
        F_WRONGEXCEPTION("LeafError*");
    }
}

TEST(Cleafcore, claefcore_delete_notInit){
    FUN();
    CLEAF_INIT_DUMMY(false);

    LEAF_DEBUG_SET_FAIL("cleafcore_delete()");

    struct cleafcore* c = cleafcore_new();

    try{
        cleafcore_delete(c);
    } catch (LeafError* e){
        FAIL() << "cleafcore_delete() does not check for initialized cleaf";
    } catch (...){
        F_WRONGEXCEPTION("LeafError*");
    }
}
