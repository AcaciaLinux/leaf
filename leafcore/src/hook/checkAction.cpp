/**
 * @file		hook/checkAction.cpp
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		The implementation of Hook::checkAction()
 * @copyright	Copyright (c) 2023 Max Kofler and the AcaciaLinux developers
 */
#include "log.h"
#include "error.h"
#include "leafdebug.h"

#include "hook.h"

bool Leaf::Hook::checkAction(Leaf::conf_tr_type& action) const{
    FUN();

    bool engage = false;

    for (const Leaf::conf_tr_type& hook_action : _actions){
        if (hook_action == action){
            engage = true;
            break;
        }
    }

    return engage;
}
