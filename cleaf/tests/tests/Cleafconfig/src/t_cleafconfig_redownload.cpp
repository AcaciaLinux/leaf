/**
 * @file		t_cleafconfig_redownload.cpp
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		Tests regarding the functions to configure leaf redownload options
 * @copyright	Copyright (c) 2022 Max Kofler and the AcaciaLinux developers
 */

#include "../t_cleafconfig.h"

#include "cleafdebug.h"
#include "leafcore.h"

#define STR_(X) #X
#define STR(X) STR_(X)

//Sets the config to true and false using the cleafconfig method and checks the result
#define CLEAFCONFIG_REDOWNLOAD_CHECK(core, cleafconfig_name, leafconfig) { \
    cleafconfig_setRedownload(core, cleafconfig_name); \
    EXPECT_EQ((int)leafconfig, (int)cleafconfig_name) << "cleafconfig_setRedownload() does not set the value correctly"; \
}

static_assert(COUNT_CLEAF_REDOWNLOAD == 3, "Amount of CLEAF_REDOWNLOAD cofigs changed");

TEST(Cleafcore, cleafconfig_setRedownload_debug){
    FUN();
    CLEAF_INIT_DUMMY(true);

    LEAF_DEBUG_SET_FAIL("cleafconfig_setRedownload()");

    try{
        cleafconfig_setRedownload(NULL, CLEAF_CONFIG_REDOWNLOAD_ALL);

        F_NOTHROW(Error::DEBUG_EXCEPTION);
    } catch (LeafError* e){
        CHECK_EC(Error::DEBUG_EXCEPTION, e);
    } catch (...){
        F_WRONGEXCEPTION("LeafError*");
    }
}

TEST(Cleafcore, cleafconfig_setRedownload_notInit){
    FUN();
    CLEAF_INIT_DUMMY(false);

    LEAF_DEBUG_SET_FAIL("cleafconfig_setRedownload()");

    struct cleafcore* c = cleafcore_new();

    try{
        cleafconfig_setRedownload(c, CLEAF_CONFIG_REDOWNLOAD_ALL);
    } catch (LeafError* e){
        FAIL() << "cleafconfig_setRedownload() does not check for initialized cleaf";
    } catch (...){
        F_WRONGEXCEPTION("LeafError*");
    }

    cleafcore_delete(c);
}

TEST(Cleafcore, cleafconfig_setRedownload_noCore){
    FUN();
    CLEAF_INIT_DUMMY(true);

    ASSERT_EQ(CLEAFCONFIG_NOCORE, cleafconfig_setRedownload(NULL, CLEAF_CONFIG_REDOWNLOAD_ALL)) << "cleafconfig_setRedownload() does not handle a NULL core correctly";

    struct cleafcore* core = new struct cleafcore;
    core->core = NULL;

    ASSERT_EQ(CLEAFCONFIG_NOCORE, cleafconfig_setRedownload(core, CLEAF_CONFIG_REDOWNLOAD_ALL)) << "cleafconfig_setRedownload() does not handle a NULL core content correctly";

    delete core;
}

TEST(Cleafconfig, cleafconfig_setRedownload){
    FUN();
    CLEAF_INIT_DUMMY(true);

    struct cleafcore* core = cleafcore_new();
    leaf_config_t& conf = ((Leafcore*)core->core)->getConfig();

    CLEAFCONFIG_REDOWNLOAD_CHECK(core, CLEAF_CONFIG_REDOWNLOAD_NONE, conf.redownload);
    CLEAFCONFIG_REDOWNLOAD_CHECK(core, CLEAF_CONFIG_REDOWNLOAD_SPECIFIED, conf.redownload);
    CLEAFCONFIG_REDOWNLOAD_CHECK(core, CLEAF_CONFIG_REDOWNLOAD_ALL, conf.redownload);

    static_assert(COUNT_CLEAF_REDOWNLOAD == 3, "Adjust this");

    cleafcore_delete(core);
}
