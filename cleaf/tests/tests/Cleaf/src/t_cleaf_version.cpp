/**
 * @file		t_cleaf_version.cpp
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		Tests regarding the functions to manage leafcore versions
 * @copyright	Copyright (c) 2022 Max Kofler and the AcaciaLinux developers
 */

#include "../t_cleaf.h"

#include "dist.h"

#include <fstream>

TEST(Cleaf, cleaf_version_major){
    FUN();

    ASSERT_EQ(LEAFCORE_V_MAJOR, cleaf_get_leafcore_major()) << "Leafcore major version doesn't match";
}

TEST(Cleaf, cleaf_version_minor){
    FUN();

    ASSERT_EQ(LEAFCORE_V_MINOR, cleaf_get_leafcore_minor()) << "Leafcore minor version doesn't match";
}

TEST(Cleaf, cleaf_version_patch){
    FUN();

    ASSERT_EQ(LEAFCORE_V_PATCH, cleaf_get_leafcore_patch()) << "Leafcore patch version doesn't match";
}
