#ifndef __FAILS_H__
#define __FAILS_H__

#include "error.h"

#define F_NOTHROW(ec) FAIL() << "LeafError with code \"" << LeafError::errorCodeToString(ec) << "\" was not thrown"
#define F_WRONGTHROW(expected, error) FAIL() << "Expected error code \"" << LeafError::errorCodeToString(expected) << "\", got \"" << error->what() << "\""
#define F_WRONGEXCEPTION(expected) FAIL() << "Expected exception of type " << expected << ", got wrong type"

#define F_THROW(ex) FAIL() << "Failed with exception: " << ex->what()

#endif