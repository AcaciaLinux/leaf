/**
 * @file        mirror/getSet.cpp
 * @author      Max Kofler (kofler.max.dev@gmail.com)
 * @brief       The file containing all the boring getter and setter stuff
 * @copyright   Copyright (c) 2023 Max Kofler and the AcaciaLinux developers
 */

#include "log.h"
#include "mirror.h"

bool Leaf::Mirror::loaded() const{
    FUN();

    return _is_loaded;
}
