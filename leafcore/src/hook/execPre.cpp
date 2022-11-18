/**
 * @file		hook/execPre.cpp
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		The implementation of Hook::execPre()
 * @copyright	Copyright (c) 2022
 */
#include "log.h"
#include "error.h"
#include "leafdebug.h"

#include "hook.h"

int Hook::execPre(leaf_config_t& config){
	FUN();

	if (_execTime == HOOK_EXEC_PRE)
		if (shouldEngage(config))
			return exec();

	return 0;
}