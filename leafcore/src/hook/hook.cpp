/**
 * @file		hook/hook.cpp
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		The implementation of the Hook class
 * @copyright	Copyright (c) 2023 Max Kofler and the AcaciaLinux developers
 */
#include "log.h"
#include "error.h"
#include "leafdebug.h"
#include "leafconfig.h"

#include "hook.h"

Leaf::Hook::Hook(){
	FUN();
}

Leaf::Hook::~Hook(){
	FUN();
}
