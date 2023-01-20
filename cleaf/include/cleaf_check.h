/**
 * @file        cleaf_check.h
 * @author      Max Kofler (kofler.max.dev@gmail.com)
 * @brief       Contains checks for initialized cleaf
 * @copyright   Copyright (c) 2022 Max Kofler and the AcaciaLinux developers
 */

#ifndef __CLEAF_CHECK_H__
#define __CLEAF_CHECK_H__

#include <iostream>

extern bool _cleaf_initialized;

#define CHECK_CLEAF_INIT(fn) { \
    if (!_cleaf_initialized){ \
        std::cerr << "[cleaf] cleaf is not initialized! Unable to perform " << fn << std::endl; \
        return; \
    } \
} \

#define CHECK_CLEAF_INIT_RET(fn, value) { \
    if (!_cleaf_initialized){ \
        std::cerr << "[cleaf] cleaf is not initialized! Unable to perform " << fn << std::endl; \
        return value; \
    } \
} \

#endif
