/**
 * @file		hook/exec.cpp
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		The implementation of Hook::exec()
 * @copyright	Copyright (c) 2022
 */
#include "log.h"
#include "error.h"
#include "leafdebug.h"

#include "hook.h"

int Hook::exec(){
	FUN();

	return system(_exec.c_str());
}
