/**
 * @file        mirror/mirror.cpp
 * @author      Max Kofler (kofler.max.dev@gmail.com)
 * @brief       The implementation of the Leaf::Mirror class
 * @copyright   Copyright (c) 2023 Max Kofler and the AcaciaLinux developers
 */
#include "log.h"
#include "error.h"
#include "leafdebug.h"
#include "leafconfig.h"

#include "mirror.h"

Leaf::Mirror::Mirror(const std::string& name, const std::string& url) : _name(name), _url(url) {
    FUN();
}
