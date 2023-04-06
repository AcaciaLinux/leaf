/**
 * @file		hook/execPost.cpp
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		The implementation of Hook::execPost()
 * @copyright	Copyright (c) 2023 Max Kofler and the AcaciaLinux developers
 */
#include "log.h"
#include "error.h"
#include "leafdebug.h"

#include "hook.h"

int Leaf::Hook::execPost(const conf_tr& conf) const{
	FUN();

	if (_execTime == HOOK_EXEC_POST)
		if (shouldEngage(conf))
			return exec(conf.parent);

	return 0;
}
