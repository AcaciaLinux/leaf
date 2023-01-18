/**
 * @file		t_cleafconfig_s.cpp
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		Tests regarding the functions to configure leaf string options
 * @copyright	Copyright (c) 2022 Max Kofler and the AcaciaLinux developers
 */

#include "../t_cleafconfig.h"

#include "cleafdebug.h"
#include "leafcore.h"

#include <assert.h>

#define STR_(X) #X
#define STR(X) STR_(X)

//Checks if a configuration is correctly marked as read only
#define CLEAFCONFIG_S_CHECK_RO(core, config_name){ \
    EXPECT_EQ(CLEAFCONFIG_RO_CONF, cleafconfig_setStringConfig(core, config_name, "test-config")) << "String config " << STR(config_name) << " is not read only"; \
}

//Checks if a configuration is correctly marked as read and write
#define CLEAFCONFIG_S_CHECK_RW(core, config_name){ \
    EXPECT_EQ(CLEAFCONFIG_OK, cleafconfig_setStringConfig(core, config_name, "test-config")) << "String config " << STR(config_name) << " is read only"; \
}

//Checks if cleafconfig_getStringConfig() matches the expected output
#define CLEAFCONFIG_S_CHECK(core, config_name, config_expected){ \
    const char* c_res = cleafconfig_getStringConfig(core, config_name); \
    std::string s(c_res); \
    EXPECT_EQ(config_expected, s) << STR(config_name) << " does not match expected " << config_expected; \
}

static_assert(COUNT_CLEAF_STRING_CONFIG == 9, "Amount of CLEAF_STRING cofigs changed");

//
// cleafconfig_getStringConfig()
//

TEST(Cleafcore, cleafconfig_setStringConfig_debug){
    FUN();
    CLEAF_INIT_DUMMY(true);

    LEAF_DEBUG_SET_FAIL("cleafconfig_setStringConfig()");

    try{
        cleafconfig_setStringConfig(NULL, CLEAF_S_CONFIG_ROOTDIR, "");

        F_NOTHROW(Error::DEBUG_EXCEPTION);
    } catch (LeafError* e){
        CHECK_EC(Error::DEBUG_EXCEPTION, e);
    } catch (...){
        F_WRONGEXCEPTION("LeafError*");
    }
}

TEST(Cleafcore, cleafconfig_setStringConfig_notInit){
    FUN();
    CLEAF_INIT_DUMMY(false);

    LEAF_DEBUG_SET_FAIL("cleafconfig_setStringonfig()");

    struct cleafcore* c = cleafcore_new();

    try{
        cleafconfig_setStringConfig(c, CLEAF_S_CONFIG_ROOTDIR, "");
    } catch (LeafError* e){
        FAIL() << "cleafconfig_setStringConfig() does not check for initialized cleaf";
    } catch (...){
        F_WRONGEXCEPTION("LeafError*");
    }

    cleafcore_delete(c);
}

TEST(Cleafcore, cleafconfig_setStringConfig_noCore){
    FUN();
    CLEAF_INIT_DUMMY(true);

    ASSERT_EQ(CLEAFCONFIG_NOCORE, cleafconfig_setStringConfig(NULL, CLEAF_S_CONFIG_ROOTDIR, "")) << "cleafconfig_setStringConfig() does not handle a NULL core correctly";

    struct cleafcore* core = new struct cleafcore;
    core->core = NULL;

    ASSERT_EQ(CLEAFCONFIG_NOCORE, cleafconfig_setStringConfig(core, CLEAF_S_CONFIG_ROOTDIR, "")) << "cleafconfig_setBoolConfig() does not handle a NULL core content correctly";

    delete core;
}

TEST(Cleafcore, cleafconfig_setStringConfig_inv_conf){
    FUN();
    CLEAF_INIT_DUMMY(true);

    struct cleafcore* core = cleafcore_new();

    ASSERT_EQ(CLEAFCONFIG_INV_CONF, cleafconfig_setStringConfig(core, COUNT_CLEAF_STRING_CONFIG, "")) << "cleafconfig_setStringConfig() does not handle an invalid (unknown) config correctly";

    cleafcore_delete(core);
}

TEST(Cleafcore, cleafconfig_setStringConfig){
    FUN();
    CLEAF_INIT_DUMMY(true);

    struct cleafcore* core = cleafcore_new();
    leaf_config_t& conf = ((Leafcore*)core->core)->getConfig();

    cleafconfig_setStringConfig(core, CLEAF_S_CONFIG_ROOTDIR, "./myRoot");
    ASSERT_EQ("./myRoot/", conf.rootDir) << "cleafconfig_setStringConfig() does not set CONFIG_ROOTDIR";

    cleafconfig_setStringConfig(core, CLEAF_S_CONFIG_PKGLISTURL, "https://myUrl");
    ASSERT_EQ("https://myUrl", conf.pkgListURL) << "cleafconfig_setStringConfing() does not set CONFIG_PKGLISTURL";

    CLEAFCONFIG_S_CHECK_RW(core, CLEAF_S_CONFIG_ROOTDIR);
    CLEAFCONFIG_S_CHECK_RW(core, CLEAF_S_CONFIG_PKGLISTURL);

    CLEAFCONFIG_S_CHECK_RO(core, CLEAF_S_CONFIG_CACHEDIR);
    CLEAFCONFIG_S_CHECK_RO(core, CLEAF_S_CONFIG_DOWNLOADDIR);
    CLEAFCONFIG_S_CHECK_RO(core, CLEAF_S_CONFIG_PACKAGESDIR);
    CLEAFCONFIG_S_CHECK_RO(core, CLEAF_S_CONFIG_CONFIGDIR);
    CLEAFCONFIG_S_CHECK_RO(core, CLEAF_S_CONFIG_INSTALLEDDIR);
    CLEAFCONFIG_S_CHECK_RO(core, CLEAF_S_CONFIG_HOOKSDIR);
    CLEAFCONFIG_S_CHECK_RO(core, CLEAF_S_CONFIG_PKGLISTPATH);

    cleafcore_delete(core);
}

//
// cleafconfig_setStringConfig()
//

TEST(Cleafcore, cleafconfig_getStringConfig_debug){
    FUN();
    CLEAF_INIT_DUMMY(true);

    LEAF_DEBUG_SET_FAIL("cleafconfig_getStringConfig()");

    try{
        cleafconfig_getStringConfig(NULL, CLEAF_S_CONFIG_ROOTDIR);

        F_NOTHROW(Error::DEBUG_EXCEPTION);
    } catch (LeafError* e){
        CHECK_EC(Error::DEBUG_EXCEPTION, e);
    } catch (...){
        F_WRONGEXCEPTION("LeafError*");
    }
}

TEST(Cleafcore, cleafconfig_getStringConfig_notInit){
    FUN();
    CLEAF_INIT_DUMMY(false);

    LEAF_DEBUG_SET_FAIL("cleafconfig_getStringConfig()");

    struct cleafcore* c = cleafcore_new();

    try{
        cleafconfig_getStringConfig(c, CLEAF_S_CONFIG_ROOTDIR);
    } catch (LeafError* e){
        FAIL() << "cleafconfig_getStringConfig() does not check for initialized cleaf";
    } catch (...){
        F_WRONGEXCEPTION("LeafError*");
    }

    cleafcore_delete(c);
}

TEST(Cleafcore, cleafconfig_getStringConfig_noCore){
    FUN();
    CLEAF_INIT_DUMMY(true);

    ASSERT_EQ(NULL, cleafconfig_getStringConfig(NULL, CLEAF_S_CONFIG_ROOTDIR)) << "cleafconfig_getStringConfig() does not handle a NULL core correctly";

    struct cleafcore* core = new struct cleafcore;
    core->core = NULL;

    ASSERT_EQ(NULL, cleafconfig_getStringConfig(core, CLEAF_S_CONFIG_ROOTDIR)) << "cleafconfig_getStringConfig() does not handle a NULL core content correctly";

    delete core;
}

TEST(Cleafcore, cleafconfig_getStringConfig_inv_conf){
    FUN();
    CLEAF_INIT_DUMMY(true);

    struct cleafcore* core = cleafcore_new();

    ASSERT_EQ(NULL, cleafconfig_getStringConfig(core, COUNT_CLEAF_STRING_CONFIG)) << "cleafconfig_getStringConfig() does not handle an invalid (unknown) config correctly";

    cleafcore_delete(core);
}

TEST(Cleafcore, cleafconfig_getStringConfig){
    FUN();
    CLEAF_INIT_DUMMY(true);

    struct cleafcore* core = cleafcore_new();
    leaf_config_t& conf = ((Leafcore*)core->core)->getConfig();

    conf.setRootDir("myRoot");

    EXPECT_EQ(std::string("myRoot/"), std::string(cleafconfig_getStringConfig(core, CLEAF_S_CONFIG_ROOTDIR)));

    CLEAFCONFIG_S_CHECK(core, CLEAF_S_CONFIG_ROOTDIR, conf.rootDir);
    CLEAFCONFIG_S_CHECK(core, CLEAF_S_CONFIG_PKGLISTURL, conf.pkgListURL);
    CLEAFCONFIG_S_CHECK(core, CLEAF_S_CONFIG_CACHEDIR, conf.cacheDir());
    CLEAFCONFIG_S_CHECK(core, CLEAF_S_CONFIG_DOWNLOADDIR, conf.downloadDir());
    CLEAFCONFIG_S_CHECK(core, CLEAF_S_CONFIG_PACKAGESDIR, conf.packagesDir());
    CLEAFCONFIG_S_CHECK(core, CLEAF_S_CONFIG_CONFIGDIR, conf.configDir());
    CLEAFCONFIG_S_CHECK(core, CLEAF_S_CONFIG_INSTALLEDDIR, conf.installedDir());
    CLEAFCONFIG_S_CHECK(core, CLEAF_S_CONFIG_HOOKSDIR, conf.hooksDir());
    CLEAFCONFIG_S_CHECK(core, CLEAF_S_CONFIG_PKGLISTPATH, conf.pkgListPath());

    static_assert(COUNT_CLEAF_STRING_CONFIG == 9, "Adjust this");

    cleafcore_delete(core);
}
