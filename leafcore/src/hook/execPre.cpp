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

int Hook::execPre(const Leafcore& core){
	FUN();

	if (_execTime == HOOK_EXEC_PRE)
		if (shouldEngage(core.getConfigC()))
			return exec(core);

	return 0;
}