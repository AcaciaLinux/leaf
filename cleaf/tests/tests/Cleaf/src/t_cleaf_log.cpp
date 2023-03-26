/**
 * @file		t_cleaf_log.cpp
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		Tests regarding the cleaf functions to manage logs
 * @copyright	Copyright (c) 2022 Max Kofler and the AcaciaLinux developers
 */

#include "../t_cleaf.h"

#include <fstream>

extern bool _cleaf_initialized;
extern std::stringstream _ss_cache;

TEST(Cleaf, cleaf_setLoglevel_debug){
    FUN();

    LEAF_DEBUG_SET_FAIL("cleaf_setLogLevel()");

    try{
        cleaf_setLogLevel(LOGLEVEL_A);

        F_NOTHROW(Error::DEBUG_EXCEPTION);
    } catch (LeafError* e){
        CHECK_EC(Error::DEBUG_EXCEPTION, e);
    } catch (...){
        F_WRONGEXCEPTION("LeafError*");
    }
}

//Tests if the check for initialized cleaf works
TEST(Cleaf, cleaf_setLogLevel_check_init){
    FUN();

    LEAF_DEBUG_SET_FAIL("cleaf_setLogLevel::afterInitCheck");

    try {
        _cleaf_initialized = false;

        ASSERT_NO_THROW(cleaf_setLogLevel(LOGLEVEL_A)) << "cleaf_setLogLevel() does not check if cleaf is initialized";
    } catch (LeafError* e){
        CHECK_EC(Error::DEBUG_EXCEPTION, e);
    } catch (...){
        F_WRONGEXCEPTION("LeafError*");
    }
}

TEST(Cleaf, cleaf_setLogLevel){
    FUN();

    _cleaf_initialized = true;

    Log::level oldLevel = hlog->getStreamConf(std::cout)->loglevel;

    loglevel l = LOGLEVEL_A;
    {//Setup the cached stream
        Log::stream_config c;
        c.loglevel = (Log::level)l;
        c.print_function_names = false;
        hlog->addStream(_ss_cache, c);
    }

    cleaf_setLogLevel(l);

    ASSERT_EQ(l, hlog->getStreamConf(std::cout)->loglevel);
    ASSERT_EQ(l, hlog->getStreamConf(_ss_cache)->loglevel);

    l = LOGLEVEL_D;
    cleaf_setLogLevel(l);

    ASSERT_EQ(l, hlog->getStreamConf(std::cout)->loglevel);
    ASSERT_EQ(l, hlog->getStreamConf(_ss_cache)->loglevel);

    l = (loglevel)oldLevel;
    cleaf_setLogLevel(l);

    ASSERT_EQ(l, hlog->getStreamConf(std::cout)->loglevel);
    ASSERT_EQ(l, hlog->getStreamConf(_ss_cache)->loglevel);

    _cleaf_initialized = false;
}

TEST(Cleaf, cleaf_clear_log){
    FUN();

    _ss_cache << "Hello" << std::endl;

    cleaf_clear_log();

    ASSERT_EQ(0, _ss_cache.str().length());
}

#include <cstring>

TEST(Cleaf, cleaf_get_log){
    FUN();

    cleaf_clear_log();

    std::string message = "Hello";

    _ss_cache.str(message);
    ASSERT_EQ(message, _ss_cache.str()) << "Stringstream does not contain right data?";

    ASSERT_EQ(0, std::strcmp(message.c_str(), cleaf_get_log())) << "cleaf_get_log does not work correctly";
}
