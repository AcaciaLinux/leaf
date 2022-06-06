/**
 * @file		hook/hook.cpp
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		The implementation of the Hook class
 * @copyright	Copyright (c) 2022
 */
#include "log.h"
#include "error.h"
#include "leafdebug.h"
#include "leafconfig.h"

#include "hook.h"

Hook::Hook(){
	FUN();
}

Hook::~Hook(){
	FUN();
}
