/**
 * @file		hook/execPost.cpp
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		The implementation of Hook::execPost()
 * @copyright	Copyright (c) 2022
 */
#include "log.h"
#include "error.h"
#include "leafdebug.h"

#include "hook.h"

int Hook::execPost(leaf_config_t& config){
	FUN();

	if (_execTime == HOOK_EXEC_POST)
		if (shouldEngage(config))
			return exec();

	return 0;
}
