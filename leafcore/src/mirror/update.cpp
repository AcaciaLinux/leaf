/**
 * @file        mirror/update.cpp
 * @author      Max Kofler (kofler.max.dev@gmail.com)
 * @brief       The implementation of Leaf::Mirror::update()
 * @copyright   Copyright (c) 2023 Max Kofler and the AcaciaLinux developers
 */
#include "log.h"
#include "error.h"
#include "leafdebug.h"

#include <fstream>
#include <nlohmann/json.hpp>
#include <sstream>

#include "mirror.h"
#include "leaffs.h"
#include "downloader.h"
#include "branchmaster.h"

void Leaf::Mirror::update(const conf_update& conf){
    FUN();
    LEAF_DEBUG_EX("Leaf::Mirror::update()");

    // Cache some directory entries
    std::filesystem::path mDir = conf.parent.mirrorsDir();
    std::filesystem::path mFile = mDir;
    mFile.append(_name + ".json");
    std::string fullName = _name + "(" + _url + ")";

    // Check if the target directory exists
    if (!LeafFS::exists(mDir))
        throw new LeafError(Error::LEAFFS_FILE_NOT_EXISTING, "Mirror directory at " + std::string(mDir));

    std::stringstream ss_mirror;

    Downloader dl(_url, ss_mirror, !conf.parent.showProgress);
    dl.init();
    size_t dRes = dl.download("Updating mirror " + _name);

    //Handle the HTTP response code
    if (dRes >= 400){
        throw new LeafError(
            Error::DL_BAD_RESPONSE,
            "When updating mirror " + fullName + ": " +
            std::to_string(dRes));
    }

    BranchResponse response;

    //Try parsing the JSON data
    try {
        nlohmann::json json_res = nlohmann::json::parse(ss_mirror, nullptr, true);

        //Transfer the data to the struct
        response.response_code = json_res.at("response_code");
        response.status = json_res.at("status");
        response.payload = json_res.at("payload").dump();

    } catch (nlohmann::json::parse_error& e){
        LOGUW("Failed to parse mirror from " + fullName + ", is the URL valid?");
        throw new LeafError(Error::JSON_PARSE, e.what());
    } catch (nlohmann::json::out_of_range& e){
        LOGUW("Failed to parse mirror response from " + fullName + ", is the URL valid?");
        throw new LeafError(Error::JSON_OUT_OF_RANGE, e.what());
    }

    //Check the response code
    if (response.response_code != 200){
        throw new LeafError(
            Error::BRANCHMASTER_ERROR,
            "When updating mirror " + fullName + ": " +
            response.payload);
    }

    //Create the output file stream, check it and write the payload
    {
        std::ofstream outFile;
        outFile.open(mFile, std::ios::out);
        if (!outFile.is_open())
            throw new LeafError(Error::OPENFILEW, "Mirror file at " + std::string(mFile));

        LOGI("Writing out mirror update to " + std::string(mFile));
        outFile << response.payload << std::endl;

        outFile.close();
    }
}
