/**
 * @file		package/pre_postinstall.cpp
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		The code for running the preinstall and postinstall scripts
 * @copyright 	Copyright (c) 2022
 */

#include "log.h"
#include "leafdebug.h"
#include "package.h"
#include "leafconfig.h"

#include "leaffs.h"
#include "util.h"

#include <filesystem>
#include <fstream>

void Package::runPreinstall(const Leaf::config& conf){
    FUN();
    LEAF_DEBUG_EX("Package::runPreinstall()");

    if (!conf.runScripts){
        LOGW("[Package][runPreinstall] WARNING: Disabled preinstall script of package " + getFullName());
        return;
    }
    runScript(conf, "preinstall.sh");
}

void Package::runPostinstall(const Leaf::config& conf){
    FUN();
    LEAF_DEBUG_EX("Package::runPostinstall()");

    if (!conf.runScripts){
        LOGW("[Package][runPostinstall] WARNING: Disabled postinstall script of package " + getFullName());
        return;
    }
    runScript(conf, "postinstall.sh");
}

void Package::runScript(const Leaf::config& conf, std::string path){
    FUN();
    LEAF_DEBUG_EX("Package::runScript()");

    LeafUtil::ensureDirs(conf);

    if (!LeafFS::exists(getExtractedDir(conf)))
        throw new LeafError(Error::PKG_NOTEXTRACTED);

    if (!LeafFS::exists(getExtractedDir(conf).append(path))){
        LOGI("[Package][runScript] Script " + std::string(getExtractedDir(conf).append(path)) + " does not exist, skipping");
        return;
    }

    //Cache the root directory
    std::string rootDir = conf.root;

    //The extracted directory relative to the chroot environment
    std::string relExtractedDir = "/" + std::string(std::filesystem::relative(this->getExtractedDir(conf), rootDir)) + "/";

    {//Create the executed script
        std::ofstream outFile(std::string(getExtractedDir(conf)) + "runscript-" + getFullName() + ".sh");
        if (!outFile.is_open())
            throw new LeafError(Error::OPENFILEW, "RunScript file for package " + getFullName());

        //Construct the script
        outFile << "#!/bin/sh" << std::endl;
        outFile << "set -e" << std::endl;
        outFile << "export PKGROOT=" + relExtractedDir << std::endl;
        outFile << relExtractedDir + path << std::endl;
        outFile << "unset PKGROOT" << std::endl;
    }

    //A breakpoint for the tests to check the script
    LEAF_DEBUG_EX("Leafcore::runScript::fileCreated");

    std::string command = "bash " + relExtractedDir + "runscript-" + getFullName() + ".sh";

    int res = LeafUtil::runCommand(conf, command, getExtractedDir(conf));

    //Check the return code
    if (res != 0)
        throw new LeafError(Error::PACKAGE_SCRIPT_FAILED, path);

}
