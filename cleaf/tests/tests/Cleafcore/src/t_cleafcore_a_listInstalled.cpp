/**
 * @file		t_cleafcore_a_listInstalled.cpp
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		Tests regarding the function cleafcore_a_listInstalled()
 * @copyright	Copyright (c) 2022 Max Kofler and the AcaciaLinux developers
 */

#include "../t_cleafcore.h"

#include "cleafdebug.h"

TEST(Cleafcore, cleafcore_a_listInstalled_debug){
    FUN();
    CLEAF_INIT_DUMMY(true);

    LEAF_DEBUG_SET_FAIL("cleafcore_a_listInstalled()");

    try{
        cleafcore_a_listInstalled(nullptr);

        F_NOTHROW(Error::DEBUG_EXCEPTION);
    } catch (LeafError* e){
        CHECK_EC(Error::DEBUG_EXCEPTION, e);
    } catch (...){
        F_WRONGEXCEPTION("LeafError*");
    }
}

TEST(Cleafcore, cleafcore_a_listInstalled_notInit){
    FUN();
    CLEAF_INIT_DUMMY(false);

    LEAF_DEBUG_SET_FAIL("cleafconfig_a_listInstalled()");

    struct cleafcore* c = cleafcore_new();

    try{
        cleafcore_a_listInstalled(c);
    } catch (LeafError* e){
        FAIL() << "cleafcore_a_listInstalled() does not check for initialized cleaf";
    } catch (...){
        F_WRONGEXCEPTION("LeafError*");
    }

    cleafcore_delete(c);
}

TEST(Cleafcore, cleafcore_a_listInstalled_noCore){
    FUN();
    CLEAF_INIT_DUMMY(true);

    ASSERT_EQ(CLEAFCORE_NOCORE, cleafcore_a_listInstalled(NULL)) << "cleafcore_a_listInstalled() does not handle a NULL core correctly";

    struct cleafcore* core = new struct cleafcore;
    core->core = NULL;

    ASSERT_EQ(CLEAFCORE_NOCORE, cleafcore_a_listInstalled(core)) << "cleafcore_a_listInstalled() does not handle a NULL core content correctly";

    delete core;
}

TEST(Cleafcore, cleafcore_a_listInstalled_call_parseInstalled){
    FUN();
    CLEAF_INIT_DUMMY(true);

    LEAF_DEBUG_SET_FAIL("Leafcore::parseInstalled()");

    struct cleafcore* core = cleafcore_new();
    cleafcore_a_update(core);

    try{
        cleafcore_a_listInstalled(core);

        F_NOTHROW(Error::DEBUG_EXCEPTION);
    } catch (LeafError* e){
        CHECK_EC(Error::DEBUG_EXCEPTION, e);
    } catch (...){
        F_WRONGEXCEPTION("LeafError*");
    }

    cleafcore_delete(core);
}

TEST(Cleafcore, cleafcore_a_listInstalled_call_a_listInstalled){
    FUN();
    CLEAF_INIT_DUMMY(true);

    LEAF_DEBUG_SET_FAIL("Leafcore::a_listInstalled()");

    struct cleafcore* core = cleafcore_new();
    cleafcore_a_update(core);

    try{
        cleafcore_a_listInstalled(core);

        F_NOTHROW(Error::DEBUG_EXCEPTION);
    } catch (LeafError* e){
        CHECK_EC(Error::DEBUG_EXCEPTION, e);
    } catch (...){
        F_WRONGEXCEPTION("LeafError*");
    }

    cleafcore_delete(core);
}
