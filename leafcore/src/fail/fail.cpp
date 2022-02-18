/**
 * @file		fail/fail.cpp
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		The implementation of fail
 * @copyright 	Copyright (c) 2022
 */

#include "log.h"
#include "fail.h"

bool leaf_fail(std::string function, std::string message){
	hlog->log(Log::E, function, message);
	return false;
}
