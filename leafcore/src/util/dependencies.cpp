/**
 * @file        util/dependencies
 * @author      Max Kofler (kofler.max.dev@gmail.com)
 * @brief       The implementation of dependency/package resolving functions
 * @copyright   Copyright (c) 2023 Max Kofler and the AcaciaLinux developers
 */
#include "log.h"
#include "error.h"
#include "leafdebug.h"

#include "util.h"

std::shared_ptr<Package> LeafUtil::resolvePackage(Leaf::config& conf, const std::string& name, bool canThrow){
    FUN();
    LEAF_DEBUG_EX("LeafUtil::resolvePackage()");

    for (Leaf::Mirror& mirror : conf.mirrors){
        if (!mirror.loaded())
            mirror.load(conf);

        std::shared_ptr<Package> package = mirror.resolve_package(name);
        if (package != nullptr)
            return package;
    }

    if (canThrow)
        throw new LeafError(Error::PKG_NOTFOUND, name);
    else
        return nullptr;
}

bool LeafUtil::resolveDependencies(Leaf::config& conf, std::deque<std::shared_ptr<Package>>& packages, std::shared_ptr<Package>& package, bool canThrow){
    FUN();
    LEAF_DEBUG_EX("LeafUtil::resolveDependencies()");

    {//Check if the work hasn't already been done...
        std::string pkgName = package->getName();
        for (size_t i = 0; i < packages.size(); i++){
            if (packages.at(i)->getName() == pkgName){
                LOGD("[LeafUtil][resolveDependencies] Skipping already resolved package " + pkgName);
                return true;
            }
        }
    }

    for (const std::string& str_dep : package->getDependencies()){

        //Before doing anything, check if the package hasn't already been added
        bool alreadyAdded = false;
        for (size_t i = 0; i < packages.size(); i++){
            if (packages.at(i)->getName() == str_dep){
                LOGD("[LeafUtil][resolveDependencies] Skipping duplicate package " + str_dep);
                alreadyAdded = true;
                break;
            }
        }
        if (alreadyAdded)
            continue;

        //Try to resolve the dependency, if not found, error out
        std::shared_ptr<Package> dependency = resolvePackage(conf, str_dep, false);
        if (dependency == nullptr)
            if (canThrow)
                throw new LeafError(Error::PACKAGE_DEP_NOT_FOUND, "Dependency " + str_dep + " for package " + package->getFullName());
            else
                return false;

        //Put the package in temporarily but store its position
        packages.push_back(dependency);
        size_t temp_pos = packages.size()-1;

        //Resolve the dependencies of the package
        resolveDependencies(conf, packages, dependency);

        //Then pull this package back to install it after its dependencies
        packages.erase(packages.begin() + temp_pos);
        packages.push_back(dependency);

    }

    {//Finally add the supplied package if not done yet
        std::string pkgName = package->getName();
        for (size_t i = 0; i < packages.size(); i++){
            if (packages.at(i)->getName() == pkgName){
                return true;
            }
        }

        packages.push_back(package);
    }

    return true;
}
