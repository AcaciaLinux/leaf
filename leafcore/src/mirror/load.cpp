/**
 * @file        mirror/load.cpp
 * @author      Max Kofler (kofler.max.dev@gmail.com)
 * @brief       The implementation of Leaf::Mirror::load()
 * @copyright   Copyright (c) 2023 Max Kofler and the AcaciaLinux developers
 */
#include "log.h"
#include "error.h"
#include "leafdebug.h"

#include "mirror.h"
#include "leaffs.h"

#include <fstream>
#include <nlohmann/json.hpp>

static std::shared_ptr<Package> parse_package(nlohmann::json& json){
    FUN();

    try {
        //Construct the new package
        std::shared_ptr<Package> newPkg = std::make_shared<Package>(json.at("name"), json.at("version"));

        //Fill the new package
        newPkg->setRealVersion(std::stoi(std::string(json.at("real_version"))));
        newPkg->setDescription(json.at("description"));

        for (nlohmann::json dep : json.at("dependencies"))
            newPkg->getDependencies().push_back(std::string(dep));

        //Check for the 'hash' tag
        if (json.count("hash") != 0)
            newPkg->set_remote_md5(json.at("hash"));
        else
            LOGD("[Leaf][Mirror][parse_package] Package " + newPkg->getName() + " does lack the 'hash' entry");

        newPkg->setFetchURL(json.at("url"));
        if (newPkg->getFetchURL().empty()){
            LOGI(newPkg->getFullName() + " gets treated as collection");
            newPkg->setIsCollection(true);
        }

        return newPkg;

    } catch (nlohmann::json::out_of_range& e){
		throw new LeafError(Error::JSON_OUT_OF_RANGE, e.what());
	} catch (nlohmann::json::exception& e) {
		throw new LeafError(Error::JSON_PARSE, e.what());
	}

    return nullptr;
}

void Leaf::Mirror::load(const config& conf){
    FUN();
    LEAF_DEBUG_EX("Leaf::Mirror::load()");

    _is_loaded = false;
    _packages.clear();

    // Cache some directory entries
    std::filesystem::path mDir = conf.mirrorsDir();
    std::filesystem::path mFile = mDir;
    mFile.append(_name + ".json");
    std::string fullName = _name + "(" + _url + ")";

    //Check if the file exists, load it and parse the json
    nlohmann::json json;
    {
        if (!LeafFS::exists(mFile))
            throw new LeafError(Error::MIRROR_CACHE_NOT_FOUND, mFile);

        std::ifstream inFile;
        inFile.open(mFile);
        if (!inFile.is_open())
            throw new LeafError(Error::OPENFILER, "Mirror file at " + std::string(mFile));

        //Parse the json data and throw an error if parsing failed
        try {
            json = nlohmann::json::parse(inFile, nullptr, true);
        } catch (nlohmann::json::parse_error& e) {
            throw new LeafError(Error::JSON_PARSE, e.what());
        }

        inFile.close();
    }

    for (nlohmann::json& json_pkg : json){
        std::shared_ptr<Package> newPkg = parse_package(json_pkg);

        _packages.push_back(newPkg);
    }

    _is_loaded = true;
}
