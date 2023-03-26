/**
 * @file        leaffs/workdir.cpp
 * @author      Max Kofler (kofler.max.dev@gmail.com)
 * @brief       The implementations for the fs commands related to the working directory
 * @copyright   Copyright (c) 2022 Max Kofler and the AcaciaLinux developers
 */
#include "log.h"
#include "error.h"
#include "leafdebug.h"

#include "leaffs.h"

//TODO: Tests
std::string LeafFS::get_workdir(bool thr){
    FUN();
    LEAF_DEBUG_EX("LeafFS::get_workdir()");

    std::error_code ec;
    std::string res = std::filesystem::current_path(ec);

    if (ec)
        if (thr)
            throw new LeafError(Error::FS_CHWDIR, ec);
        else
            return "";

    return res;
}

//TODO: Tests
bool LeafFS::change_workdir(const std::filesystem::path& path, bool thr){
    FUN();
    LEAF_DEBUG_EX("LeafFS::change_workdir()");

    std::error_code ec;
    std::filesystem::current_path(path, ec);

    if (ec)
        if (thr)
            throw new LeafError(Error::FS_CHWDIR, ec);
        else
            return false;

    return true;
}
