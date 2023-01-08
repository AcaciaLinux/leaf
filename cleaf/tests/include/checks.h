#ifndef __CHECKS_H__
#define __CHECKS_H__

#include "fails.h"

#define CHECK_EC(expected, error) if (expected != error->getErrorCode()) F_WRONGTHROW(expected, error)

#endif