/**
 * @file        cleafdebug/cleafdebug.cpp
 * @author      Max Kofler (kofler.max.dev@gmail.com)
 * @brief       The implementation of the cleafdebug.h header
 * @copyright   Copyright (c) 2022 Max Kofler and the AcaciaLinux developers
 */

#include "log.h"
#include "cleafdebug.h"

extern bool _cleaf_initialized;

CLeafInitDebugDummy::CLeafInitDebugDummy(bool state){
    FUN();
    LOGD("Setting up cleaf debugging initialization from " + std::to_string(_cleaf_initialized) + " to " + std::to_string(state));

    _prevState = _cleaf_initialized;
    _cleaf_initialized = state;
}

CLeafInitDebugDummy::~CLeafInitDebugDummy(){
    FUN();
    LOGD("Restoring old cleaf init status to " + std::to_string(_prevState));

    _cleaf_initialized = _prevState;
}
