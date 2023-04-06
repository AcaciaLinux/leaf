/**
 * @file		hook/exec.cpp
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		The implementation of Hook::exec()
 * @copyright	Copyright (c) 2023 Max Kofler and the AcaciaLinux developers
 */
#include "log.h"
#include "error.h"
#include "leafdebug.h"
#include "util.h"

#include "hook.h"

int Leaf::Hook::exec(const Leaf::config& conf) const{
	FUN();

	return LeafUtil::runCommand(conf, _exec, conf.root);
}
