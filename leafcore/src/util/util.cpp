/**
 * @file        util/util.cpp
 * @author      Max Kofler (kofler.max.dev@gmail.com)
 * @brief       The implementation of some functions of the util.h header
 * @copyright   Copyright (c) 2022 Max Kofler and the AcaciaLinux developers
 */
#include "log.h"
#include "error.h"
#include "leafdebug.h"

#include "util.h"

std::string LeafUtil::bytesToHuman(uint64_t byteCount){
    FUN();
    LEAF_DEBUG_EX("LeafUtil::bytesToHuman()");

    const char *suffix[] = {"B", "KB", "MB", "GB", "TB"};
    char length = sizeof(suffix) / sizeof(suffix[0]);

    int i = 0;
    double dblBytes = byteCount;

    if (byteCount > 1024) {
        for (i = 0; (byteCount / 1024) > 0 && i<length-1; i++, byteCount /= 1024)
            dblBytes = byteCount / 1024.0;
    }

    char buf [10];
    sprintf(buf, "%.02lf ", dblBytes);
    return std::string(buf) + std::string(suffix[i]);
}
