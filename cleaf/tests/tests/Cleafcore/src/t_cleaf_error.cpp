/**
 * @file		t_cleaf_error.cpp
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		Tests regarding the functions to handle Leafcore errors
 * @copyright	Copyright (c) 2022 Max Kofler and the AcaciaLinux developers
 */

#include "../t_cleafcore.h"

//
// cleafcore_getError()
//

TEST(Cleafcore, cleafcore_getError_debug){
    FUN();

    LEAF_DEBUG_SET_FAIL("cleafcore_getError()");

    try{
        struct cleafcore c;
        c.core = NULL;
        c.error = NULL;

        cleafcore_getError(&c);

        F_NOTHROW(Error::DEBUG_EXCEPTION);
    } catch (LeafError* e){
        CHECK_EC(Error::DEBUG_EXCEPTION, e);
    } catch (...){
        F_WRONGEXCEPTION("LeafError*");
    }
}

TEST(Cleafcore, cleafcore_getError){
    FUN();

    struct cleafcore c;
    c.core = NULL;
    c.error = NULL;

    ASSERT_EQ(0, cleafcore_getError(&c)) << "No error does not return error code 0";

    c.error = new LeafError(Error::NONE);

    ASSERT_EQ(Error::NONE, ((LeafError*)c.error)->getErrorCode()) << "Error code does not get called correctly";

    delete (LeafError*)c.error;
}

//
// cleafcore_getErrorString()
//

TEST(Cleafcore, cleafcore_getErrorString_debug){
    FUN();

    LEAF_DEBUG_SET_FAIL("cleafcore_getErrorString()");

    try{
        struct cleafcore c;
        c.core = NULL;
        c.error = NULL;

        cleafcore_getErrorString(&c);

        F_NOTHROW(Error::DEBUG_EXCEPTION);
    } catch (LeafError* e){
        CHECK_EC(Error::DEBUG_EXCEPTION, e);
    } catch (...){
        F_WRONGEXCEPTION("LeafError*");
    }
}

TEST(Cleafcore, cleafcore_getErrorString){
    FUN();

    struct cleafcore c;
    c.core = NULL;
    c.error = NULL;

    ASSERT_EQ(std::string("No error"), std::string(cleafcore_getErrorString(&c))) << "No error does not return 'No error'";

    c.error = new LeafError(Error::NODB);

    ASSERT_EQ("Database is not accessible", ((LeafError*)c.error)->getErrorCodeMessage()) << "Error string does not match error code (Or the message has changed)";

    delete (LeafError*)c.error;
}

//
// cleafcore_clear_error()
//

TEST(Cleafcore, cleafcore_clear_error_debug){
    FUN();

    LEAF_DEBUG_SET_FAIL("cleafcore_clear_error()");

    try{
        struct cleafcore c;
        c.core = NULL;
        c.error = NULL;

        cleafcore_clear_error(&c);

        F_NOTHROW(Error::DEBUG_EXCEPTION);
    } catch (LeafError* e){
        CHECK_EC(Error::DEBUG_EXCEPTION, e);
    } catch (...){
        F_WRONGEXCEPTION("LeafError*");
    }
}

//Checks if the cleafcore_clear_error() function calls the LeafError destructor
TEST(Cleafcore, cleafcore_clear_error_delete){
    FUN();

    LEAF_DEBUG_SET_FAIL("LeafError::~LeafError()");

    try{
        struct cleafcore c;
        c.core = NULL;
        c.error = new LeafError(Error::NONE);

        cleafcore_clear_error(&c);

        F_NOTHROW(Error::DEBUG_EXCEPTION);
    } catch (LeafError* e){
        CHECK_EC(Error::DEBUG_EXCEPTION, e);
    } catch (...){
        F_WRONGEXCEPTION("LeafError*");
    }
}
