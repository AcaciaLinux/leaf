/**
 * @file        cleaf/cleaf_pkgs_to_v.cpp
 * @author      Max Kofler (kofler.max.dev@gmail.com)
 * @brief       The implementation of the function cleaf_pkgs_to_v()
 * @copyright   Copyright (c) 2022 Max Kofler and the AcaciaLinux developers
 */

#include "log.h"

#include "cleaf.h"

std::deque<std::string> cleaf_pkgs_to_v(uint32_t len_pkgs, const char* pkgs[]){
    std::deque<std::string> res;

    for (uint32_t i = 0; i < len_pkgs; i++){
        res.push_back(std::string(pkgs[i]));
    }

    return res;
}
