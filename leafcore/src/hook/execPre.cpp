/**
 * @file		hook/execPre.cpp
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		The implementation of Hook::execPre()
 * @copyright	Copyright (c) 2023 Max Kofler and the AcaciaLinux developers
 */
#include "log.h"
#include "error.h"
#include "leafdebug.h"

#include "hook.h"

int Leaf::Hook::execPre(const conf_tr& conf) const{
	FUN();

	if (_execTime == HOOK_EXEC_PRE)
		if (shouldEngage(conf))
			return exec(conf.parent);

	return 0;
}