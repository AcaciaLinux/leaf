/**
 * @file        cleafdebug.h
 * @author      Max Kofler (kofler.max.dev@gmail.com)
 * @brief       Contains some handy features for debugging cleaf
 * @copyright   Copyright (c) 2022 Max Kofler and the AcaciaLinux developers
 */

#ifndef __CLEAF_DEBUG_H__
#define __CLEAF_DEBUG_H__

#include <vector>
#include <string>

#include "error.h"

#ifdef DEBUG

    class CLeafInitDebugDummy{

    public:
        CLeafInitDebugDummy(bool state);
        ~CLeafInitDebugDummy();

    private:
        bool		_prevState = false;
    };

    #define CLEAF_INIT_DUMMY(state) CLeafInitDebugDummy cleafInitDebugDummy(state)

#else
    #define CLEAF_INIT_DUMMY
#endif

#endif
