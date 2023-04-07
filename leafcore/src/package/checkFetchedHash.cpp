/**
 * @file		package/checkFetchedHash.cpp
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		The implementation of Package::checkFetchedHash()
 * @copyright	Copyright (c) 2022
 */
#include "log.h"
#include "error.h"
#include "leafdebug.h"
#include "util.h"

#include "package.h"

#include <filesystem>

//TODO: Tests
bool Package::checkFetchedHash(const Leaf::config& conf, bool noThrow){
    FUN();
    LEAF_DEBUG_EX("Package::checkFetchedHash()");

    //Check the md5 hash
    if (_remote_md5.length() != 0 && _remote_md5 != _local_md5){

        LOGUE(
            "The package " + getFullName()  + " has an invalid hash: \n" +
            "----------------------------------------\n" +
            "remote: " + _remote_md5 + "\n" +
            "local:  " + _local_md5 + "\n" +
            "----------------------------------------");

        if (!LeafUtil::askUserOK(conf, "It might be corrupted, do you want to continue anyway?", false)){

            {//Remove the corrupted file
                LOGF("[Package][fetch] Removing corrupted package file " + std::string(getDownloadPath(conf)));
                std::error_code ec;
                std::filesystem::remove_all(getDownloadPath(conf), ec);

                if (ec){
                    throw new LeafError(Error::REMOVEFILE, ec);
                }
            }

            if (noThrow)
                return false;
            else
                throw new LeafError(Error::USER_DISAGREE, "Continue installing a package with invalid MD5 hash");
        }

        return false;
    }

    return true;
}
