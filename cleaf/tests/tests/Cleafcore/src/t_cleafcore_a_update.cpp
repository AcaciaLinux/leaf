/**
 * @file		t_cleafcore_a_update.cpp
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		Tests regarding the function cleafcore_a_update()
 * @copyright	Copyright (c) 2022 Max Kofler and the AcaciaLinux developers
 */

#include "../t_cleafcore.h"

#include "cleafdebug.h"

TEST(Cleafcore, cleafcore_a_update_debug){
    FUN();
    CLEAF_INIT_DUMMY(true);

    LEAF_DEBUG_SET_FAIL("cleafcore_a_update()");

    try{
        cleafcore_a_update(NULL);

        F_NOTHROW(Error::DEBUG_EXCEPTION);
    } catch (LeafError* e){
        CHECK_EC(Error::DEBUG_EXCEPTION, e);
    } catch (...){
        F_WRONGEXCEPTION("LeafError*");
    }
}

TEST(Cleafcore, cleafcore_a_update_notInit){
    FUN();
    CLEAF_INIT_DUMMY(false);

    LEAF_DEBUG_SET_FAIL("cleafconfig_setStringonfig()");

    struct cleafcore* c = cleafcore_new();

    try{
        cleafcore_a_update(c);
    } catch (LeafError* e){
        FAIL() << "cleafcore_a_update() does not check for initialized cleaf";
    } catch (...){
        F_WRONGEXCEPTION("LeafError*");
    }

    cleafcore_delete(c);
}

TEST(Cleafcore, cleafcore_a_update_noCore){
    FUN();
    CLEAF_INIT_DUMMY(true);

    ASSERT_EQ(CLEAFCORE_NOCORE, cleafcore_a_update(NULL)) << "cleafcore_a_update() does not handle a NULL core correctly";

    struct cleafcore* core = new struct cleafcore;
    core->core = NULL;

    ASSERT_EQ(CLEAFCORE_NOCORE, cleafcore_a_update(core)) << "cleafcore_a_update() does not handle a NULL core content correctly";

    delete core;
}

//Tests if the function call the appropriate leafcore function
TEST(Cleafcore, cleafcore_a_update_call){
    FUN();
    CLEAF_INIT_DUMMY(true);

    LEAF_DEBUG_SET_FAIL("Leafcore::a_update()");

    struct cleafcore* core = cleafcore_new();

    try{
        cleafcore_a_update(core);

        F_NOTHROW(Error::DEBUG_EXCEPTION);
    } catch (LeafError* e){
        CHECK_EC(Error::DEBUG_EXCEPTION, e);
    } catch (...){
        F_WRONGEXCEPTION("LeafError*");
    }

    cleafcore_delete(core);
}
