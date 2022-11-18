/**
 * @file		hook/checkAction.cpp
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		The implementation of Hook::checkAction()
 * @copyright	Copyright (c) 2022
 */
#include "log.h"
#include "error.h"
#include "leafdebug.h"

#include "hook.h"

bool Hook::checkAction(leaf_action& action){
	FUN();

	bool engage = false;

	for (leaf_action& hook_action : _actions){
		if (hook_action == action){
			engage = true;
			break;
		}
	}

	return engage;
}
