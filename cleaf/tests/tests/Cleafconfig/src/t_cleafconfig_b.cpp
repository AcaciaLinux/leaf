/**
 * @file		t_cleafconfig_b.cpp
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		Tests regarding the functions to configure leaf boolean options
 * @copyright	Copyright (c) 2022 Max Kofler and the AcaciaLinux developers
 */

#include "../t_cleafconfig.h"

#include "cleafdebug.h"

#include <assert.h>

static_assert(COUNT_CLEAF_BOOL_CONFIG == 9, "Amount of CLEAF_BOOL cofigs changed");

//
// cleafconfig_getBoolConfig()
//

TEST(Cleafcore, cleafconfig_setBoolConfig_debug){
    FUN();
    CLEAF_INIT_DUMMY(true);

    LEAF_DEBUG_SET_FAIL("cleafconfig_setBoolConfig()");

    try{
        cleafconfig_setBoolConfig(NULL, CLEAF_B_CONFIG_NOASK, CLEAFCONFIG_B_TRUE);

        F_NOTHROW(Error::DEBUG_EXCEPTION);
    } catch (LeafError* e){
        CHECK_EC(Error::DEBUG_EXCEPTION, e);
    } catch (...){
        F_WRONGEXCEPTION("LeafError*");
    }
}

TEST(Cleafcore, cleafconfig_setBoolConfig_notInit){
    FUN();
    CLEAF_INIT_DUMMY(false);

    LEAF_DEBUG_SET_FAIL("cleafconfig_setBoolConfig()");

    struct cleafcore* c = cleafcore_new();

    try{
        cleafconfig_setBoolConfig(c, CLEAF_B_CONFIG_FORCE, CLEAFCONFIG_B_TRUE);
    } catch (LeafError* e){
        FAIL() << "cleafconfig_setBoolConfig() does not check for initialized cleaf";
    } catch (...){
        F_WRONGEXCEPTION("LeafError*");
    }

    cleafcore_delete(c);
}

TEST(Cleafcore, cleafconfig_setBoolConfig_noCore){
    FUN();
    CLEAF_INIT_DUMMY(true);

    ASSERT_EQ(CLEAFCONFIG_NOCORE, cleafconfig_setBoolConfig(NULL, CLEAF_B_CONFIG_FORCE, CLEAFCONFIG_B_TRUE)) << "cleafconfig_setBoolConfig() does not handle a NULL core correctly";

    struct cleafcore* core = new struct cleafcore;
    core->core = NULL;

    ASSERT_EQ(CLEAFCONFIG_NOCORE, cleafconfig_setBoolConfig(core, CLEAF_B_CONFIG_FORCE, CLEAFCONFIG_B_TRUE)) << "cleafconfig_setBoolConfig() does not handle a NULL core content correctly";

    delete core;
}

TEST(Cleafcore, cleafconfig_setBoolConfig_inv_conf){
    FUN();
    CLEAF_INIT_DUMMY(true);

    struct cleafcore* core = cleafcore_new();

    ASSERT_EQ(CLEAFCONFIG_INV_CONF, cleafconfig_setBoolConfig(core, COUNT_CLEAF_BOOL_CONFIG, CLEAFCONFIG_B_FALSE)) << "cleafconfig_setBoolConfig() does not handle an invalid (unknown) config correctly";

    cleafcore_delete(core);
}

//
// cleafconfig_getBoolConfig()
//

TEST(Cleafcore, cleafconfig_getBoolConfig_debug){
    FUN();
    CLEAF_INIT_DUMMY(true);

    LEAF_DEBUG_SET_FAIL("cleafconfig_getBoolConfig()");

    try{
        cleafconfig_getBoolConfig(NULL, CLEAF_B_CONFIG_NOASK);

        F_NOTHROW(Error::DEBUG_EXCEPTION);
    } catch (LeafError* e){
        CHECK_EC(Error::DEBUG_EXCEPTION, e);
    } catch (...){
        F_WRONGEXCEPTION("LeafError*");
    }
}

TEST(Cleafcore, cleafconfig_getBoolConfig_notInit){
    FUN();
    CLEAF_INIT_DUMMY(false);

    LEAF_DEBUG_SET_FAIL("cleafconfig_getBoolConfig()");

    struct cleafcore* c = cleafcore_new();

    try{
        cleafconfig_getBoolConfig(c, CLEAF_B_CONFIG_FORCE);
    } catch (LeafError* e){
        FAIL() << "cleafconfig_getBoolConfig() does not check for initialized cleaf";
    } catch (...){
        F_WRONGEXCEPTION("LeafError*");
    }

    cleafcore_delete(c);
}

TEST(Cleafcore, cleafconfig_getBoolConfig_noCore){
    FUN();
    CLEAF_INIT_DUMMY(true);

    ASSERT_EQ(CLEAFCONFIG_NOCORE, cleafconfig_getBoolConfig(NULL, CLEAF_B_CONFIG_FORCE)) << "cleafconfig_getBoolConfig() does not handle a NULL core correctly";

    struct cleafcore* core = new struct cleafcore;
    core->core = NULL;

    ASSERT_EQ(CLEAFCONFIG_NOCORE, cleafconfig_getBoolConfig(core, CLEAF_B_CONFIG_FORCE)) << "cleafconfig_getBoolConfig() does not handle a NULL core content correctly";

    delete core;
}

TEST(Cleafcore, cleafconfig_getBoolConfig_inv_conf){
    FUN();
    CLEAF_INIT_DUMMY(true);

    struct cleafcore* core = cleafcore_new();

    ASSERT_EQ(CLEAFCONFIG_INV_CONF, cleafconfig_getBoolConfig(core, COUNT_CLEAF_BOOL_CONFIG)) << "cleafconfig_getBoolConfig() does not handle an invalid (unknown) config correctly";

    cleafcore_delete(core);
}
