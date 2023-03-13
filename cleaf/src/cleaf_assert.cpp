/**
 * @file		cleaf/cleaf_assert.cpp
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		This file contains some assert to check cleaf is up-to-date with the leaf config struct
 * @copyright	Copyright (c) 2022
 */
#include "log.h"
#include "leafcore.h"

//Assert some things in the leaf_config_t, so cleaf doesn't get out of date accidentally

static_assert(sizeof(leaf_config_t) == 240, "[cleaf] Size of leaf_config_t changed, adjust cleafconfig!");
static_assert(COUNT_CONFIG_V == 4, "[cleaf] Verbosity options changes, adjust cleafconfig!");
static_assert(COUNT_CONFIG_REDOWNLOAD == 3, "[cleaf] Redownload options changed, adjust cleafconfig!");
static_assert(COUNT_CONFIG_ACTION == 7, "[cleaf] Action options changed, adjust cleafconfig!");
