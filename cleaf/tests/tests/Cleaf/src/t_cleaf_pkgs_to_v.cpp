/**
 * @file		t_cleaf_pkgs_to_v.cpp
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		Tests regarding the cleaf function cleaf_pkgs_to_v()
 * @copyright	Copyright (c) 2022 Max Kofler and the AcaciaLinux developers
 */

#include "../t_cleaf.h"

#include <fstream>

TEST(Cleaf, cleaf_pkgs_to_v){
    FUN();

    const char* pkgs[2] = {"myPkg", "second"};
    std::deque<std::string> res = cleaf_pkgs_to_v(2, pkgs);
    ASSERT_EQ("myPkg", res.at(0));
    ASSERT_EQ("second", res.at(1));
}
