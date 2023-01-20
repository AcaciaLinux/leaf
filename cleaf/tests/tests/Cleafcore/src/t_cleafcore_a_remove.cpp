/**
 * @file		t_cleafcore_a_remove.cpp
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		Tests regarding the function cleafcore_a_remove()
 * @copyright	Copyright (c) 2022 Max Kofler and the AcaciaLinux developers
 */

#include "../t_cleafcore.h"

#include "cleafdebug.h"

TEST(Cleafcore, cleafcore_a_remove_debug){
    FUN();
    CLEAF_INIT_DUMMY(true);

    LEAF_DEBUG_SET_FAIL("cleafcore_a_remove()");

    try{
        cleafcore_a_remove(NULL, 0, NULL);

        F_NOTHROW(Error::DEBUG_EXCEPTION);
    } catch (LeafError* e){
        CHECK_EC(Error::DEBUG_EXCEPTION, e);
    } catch (...){
        F_WRONGEXCEPTION("LeafError*");
    }
}

TEST(Cleafcore, cleafcore_a_remove_notInit){
    FUN();
    CLEAF_INIT_DUMMY(false);

    LEAF_DEBUG_SET_FAIL("cleafconfig_a_remove()");

    struct cleafcore* c = cleafcore_new();

    try{
        cleafcore_a_remove(c, 0, NULL);
    } catch (LeafError* e){
        FAIL() << "cleafcore_a_remove() does not check for initialized cleaf";
    } catch (...){
        F_WRONGEXCEPTION("LeafError*");
    }

    cleafcore_delete(c);
}

TEST(Cleafcore, cleafcore_a_remove_noCore){
    FUN();
    CLEAF_INIT_DUMMY(true);

    ASSERT_EQ(CLEAFCORE_NOCORE, cleafcore_a_remove(NULL, 0, NULL)) << "cleafcore_a_remove() does not handle a NULL core correctly";

    struct cleafcore* core = new struct cleafcore;
    core->core = NULL;

    ASSERT_EQ(CLEAFCORE_NOCORE, cleafcore_a_remove(core, 0, NULL)) << "cleafcore_a_remove() does not handle a NULL core content correctly";

    delete core;
}

//Tests if the function call the appropriate leafcore function
TEST(Cleafcore, cleafcore_a_remove_call){
    FUN();
    CLEAF_INIT_DUMMY(true);
}
TEST(Cleafcore, cleafcore_a_remove_call_parseInstalled){
    FUN();
    CLEAF_INIT_DUMMY(true);

    LEAF_DEBUG_SET_FAIL("Leafcore::parseInstalled()");

    struct cleafcore* core = cleafcore_new();

    try{
        cleafcore_a_remove(core, 0, NULL);

        F_NOTHROW(Error::DEBUG_EXCEPTION);
    } catch (LeafError* e){
        CHECK_EC(Error::DEBUG_EXCEPTION, e);
    } catch (...){
        F_WRONGEXCEPTION("LeafError*");
    }

    cleafcore_delete(core);
}

TEST(Cleafcore, cleafcore_a_remove_call_parsePackageList){
    FUN();
    CLEAF_INIT_DUMMY(true);

    LEAF_DEBUG_SET_FAIL("Leafcore::parsePackageList()");

    struct cleafcore* core = cleafcore_new();

    try{
        cleafcore_a_remove(core, 0, NULL);

        F_NOTHROW(Error::DEBUG_EXCEPTION);
    } catch (LeafError* e){
        CHECK_EC(Error::DEBUG_EXCEPTION, e);
    } catch (...){
        F_WRONGEXCEPTION("LeafError*");
    }

    cleafcore_delete(core);
}

TEST(Cleafcore, cleafcore_a_remove_call_a_remove){
    FUN();
    CLEAF_INIT_DUMMY(true);

    LEAF_DEBUG_SET_FAIL("Leafcore::a_remove()");

    struct cleafcore* core = cleafcore_new();

    try{
        cleafcore_a_remove(core, 0, NULL);

        F_NOTHROW(Error::DEBUG_EXCEPTION);
    } catch (LeafError* e){
        CHECK_EC(Error::DEBUG_EXCEPTION, e);
    } catch (...){
        F_WRONGEXCEPTION("LeafError*");
    }

    cleafcore_delete(core);
}
