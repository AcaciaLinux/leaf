/**
 * @file        mirror/resolve_package.cpp
 * @author      Max Kofler (kofler.max.dev@gmail.com)
 * @brief       The implementation of Leaf::Mirror::resolve_package()
 * @copyright   Copyright (c) 2023 Max Kofler and the AcaciaLinux developers
 */
#include "log.h"
#include "error.h"
#include "leafdebug.h"

#include "mirror.h"

std::shared_ptr<Package> Leaf::Mirror::resolve_package(const std::string& name) const{
    FUN();
    LEAF_DEBUG_EX("Leaf::Mirror::resolve_package()");

    if (!loaded())
        throw new LeafError(Error::MIRROR_NOT_LOADED, _name);

    for (const std::shared_ptr<Package>& package : _packages){
        if (package->getName() == name){
            LOGI("[Leaf][Mirror][resolve_package] Mirror " + _name + " has package " + package->toString());
            return package;
        }
    }

    return  nullptr;
}
