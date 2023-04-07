/**
 * @file		package/fetch.cpp
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		The code for fetching a package from  the server
 * @copyright 	Copyright (c) 2022
 */

#include "log.h"
#include "util.h"
#include "leafdebug.h"
#include "package.h"
#include "downloader.h"
#include "md5.h"
#include "leaffs.h"
#include "branchmaster.h"

#include <fstream>
#include <filesystem>

void Package::fetch(const Leaf::config& conf){
    FUN();
    LEAF_DEBUG_EX("Package::fetch()");

    if (_isCollection){
        LOGI("[Package][fetch] Skipping fetch of collection " + getFullName());
        return;
    }

    LeafUtil::ensureDirs(conf);

    std::string destination = getDownloadPath(conf);

    if (destination.empty())
        throw new LeafError(Error::PACKAGE_FETCH_DEST_EMPTY, getFullName());

    //Check if the package has an expected md5 hash
    if (_remote_md5.length() == 0){
        if (!LeafUtil::askUserOK(conf, "The package " + getFullName() + " does not have an expected MD5 hash, do you want to continue anyway?", false)){
            throw new LeafError(Error::USER_DISAGREE, "Continue installing without MD5 checks");
        }
    }

    if (std::filesystem::exists(getDownloadPath(conf))){
        bool skip = true;

        std::ifstream inFile;
        inFile.open(getDownloadPath(conf), std::ios::binary);
        if (!inFile.is_open())
            throw new LeafError(Error::OPENFILER, "Download destination for validation " + std::string(getDownloadPath(conf)));

        std::string existingHash = md5(inFile);

        if (_remote_md5 != existingHash){
            LOGI("[Package][fetch] Existing package file at " + std::string(getDownloadPath(conf)) + " differs from remote, redownloading...");
            removeFile(getDownloadPath(conf), true);
            skip = false;
        }

        if (skip){
            LOGI("[Package][fetch] Skipping download of existing validated package file " + std::string(getDownloadPath(conf)));
            _local_md5 = existingHash;
            return;
        }
    }

    LOGD("Opening destination file " + destination + "...");
    //Create and open the destination file
    std::ofstream outFile;
    outFile.open(destination, std::ios::trunc | std::ios::binary);

    //Check if the destination file is open
    if (!outFile.is_open())
        throw new LeafError(Error::OPENFILEW, "Download for " + getFullName() + ": " + destination);

    //Create the downloader instance
    Downloader dl(getFetchURL(), outFile, !conf.showProgress);
    dl.init();

    LOGI("[Package][fetch] Downloading package " + getFullName() + " to " + destination);

    //Download the package file
    if (!conf.showProgress)
        LOGU("Downloading package " + getFullName() + "...");

    size_t dRes = dl.download("Downloading " + getFullName());
    _local_md5 = dl.getMD5();

    outFile.close();

    //If everything is ok, return
    if (dRes < 400)
        return;

    std::ifstream ecFile(destination, std::ios::in);
    if (!ecFile.is_open()){
        ecFile.close();
        throw new LeafError(Error::OPENFILER, destination + "for reading back branchmaster error code");
    }

    std::string resString;
    if (!getline(ecFile, resString)){
        ecFile.close();
        throw new LeafError(Error::OPENFILER, destination + "while reading back branchmaster error code");
    }

    BranchMaster::ec ec = BranchMaster::parseEC(dRes, resString);

    if (ec == BranchMaster::E_NONE)
        LOGUW("The package fetch resulted in a HTTP error code, but the code parsing resulted in no error, something suspicous could be going on!");
    else
        throw new LeafError(Error::BRANCHMASTER_ERROR, BranchMaster::getECString(ec));
}
