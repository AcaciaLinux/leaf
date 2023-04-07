/**
 * @file		package/extract.cpp
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		The code for extracting a downloaded package for further processing
 * @copyright 	Copyright (c) 2022
 */

#include "log.h"
#include "leafdebug.h"
#include "package.h"
#include "leafarchive.h"
#include "leafconfig.h"
#include "md5.h"
#include "util.h"

#include <filesystem>
#include <fstream>

//TODO: Tests

void Package::extract(const Leaf::config& conf){
    FUN();
    LEAF_DEBUG_EX("Package::extract()");

    if (_isCollection){
        LOGI("[Package][extract] Skipping extracting of collection " + getFullName());
        return;
    }

    LeafUtil::ensureDirs(conf);

    std::string sourcePath;
    if (_isLocal)
        sourcePath = _localSourcePath;
    else
        sourcePath = getDownloadPath(conf);

    std::string extractedDir = getExtractedDir(conf);

    std::error_code errCode;
    //If the directory exists, delete it
    if (std::filesystem::exists(extractedDir)){
        LOGD("[Package][extract] Destination directory " + extractedDir + " exists, deleting...");
        std::error_code errCode;
        std::filesystem::remove_all(extractedDir, errCode);
        if (errCode)
            throw new LeafError(Error::REMOVEDIR, extractedDir);
    }

    {//Generate the hash of the input file
        std::ifstream inFile;
        inFile.open(sourcePath, std::ios::binary);

        if (!inFile.is_open())
            throw new LeafError(Error::OPENFILER, "Package source file for validation " + sourcePath);

        _installed_md5 = md5(inFile);

        inFile.close();
    }

    //Create the archive instance
    LeafArchive archive;

    //Load the archive
    archive.load(sourcePath);

    //Extract the archive
    LOGD("[Package][extract] Extracting archive " + sourcePath + " into " + extractedDir + "...");
    archive.extract(conf.packagesDir());
}
