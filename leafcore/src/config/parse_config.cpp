/**
 * @file        leafconfig/parse_config.cpp
 * @author      Max Kofler (kofler.max.dev@gmail.com)
 * @brief       The implementation of leafconfig.h -> parse_config()
 * @copyright   Copyright (c) 2023 Max Kofler and the AcaciaLinux developers
 */
#include "log.h"
#include "error.h"
#include "leafdebug.h"

#include "config.h"
#include "leaffs.h"

#include <fstream>
#include <inicpp.h>

static ini::IniField tryConf(const ini::IniSection& ini, const std::string& key, const std::string& def){
    FUN();

    if (ini.count(key) == 0) {
        LOGI("[Leaf config] Config does not contain key '" + key + "', using default: '" + def + "'");
        return def;
    }

    LOGI("[Leaf config] Using config option '" + key + "': '" + ini.at(key).as<std::string>() + "'");
    return ini.at(key);
}

//TOOD: Tests
bool Leaf::parse_config(config& c, std::istream& in){
    FUN();
    LEAF_DEBUG_EX("parse_config()");

    ini::IniFile config;
    config.decode(in);

    //Warn the user if no leaf section is present
    if (config.count("leaf") == 0){
        LOGUW("The config file does not contain a [leaf] section, it will probably not be useful!");
    } else {
        c.root = tryConf(config["leaf"], "root", c.root).as<std::string>();
    }

    for (auto mirror : config["mirrors"]){
        LOGI("[Leaf config] Mirror '" + mirror.first + "' -> " + mirror.second.as<std::string>());
        c.mirrors.push_back(Leaf::Mirror(mirror.first, mirror.second.as<std::string>()));
    }

    return true;
}
