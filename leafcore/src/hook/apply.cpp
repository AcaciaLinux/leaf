/**
 * @file		hook/apply.cpp
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		The implementation of Hook::apply()
 * @copyright	Copyright (c) 2022
 */
#include "log.h"
#include "error.h"
#include "leafdebug.h"
#include "leafconfig.h"

#include "hook.h"

#include <algorithm>

static leaf_action parseAction(std::string& str);

void Hook::apply(std::map<std::string, std::string>& entries){
	FUN();
	LEAF_DEBUG_EX("Hook::apply()");

	if (entries.count("action") <= 0)
		throw new LeafError(Error::HOOK_REQUIRED_VALUE, "'action' in " + _filePath);

	if (entries.count("packages") <= 0)
		throw new LeafError(Error::HOOK_REQUIRED_VALUE, "'packages' in " + _filePath);

	if (entries.count("when") <= 0)
		throw new LeafError(Error::HOOK_REQUIRED_VALUE, "'when' in " + _filePath);

	if (entries.count("rundep") <= 0)
		throw new LeafError(Error::HOOK_REQUIRED_VALUE, "'rundep' in " + _filePath);

	if (entries.count("exec") <= 0)
		throw new LeafError(Error::HOOK_REQUIRED_VALUE, "'exec' in " + _filePath);

	{	//Parse the 'action' value
		std::string actionString = entries["action"];
		actionString.erase(std::remove_if(actionString.begin(), actionString.begin(), isspace), actionString.begin());

		std::string buf;
		for (char c : actionString){
			if (c == ','){

				leaf_action action = parseAction(buf);
				if (action != ACTION_NONE){
					LOGF("[Hook][apply] Adding action " + std::to_string(action) + " to hook");
					_actions.push_back(action);
				}
				
				buf.clear();
				continue;
			}

			buf += c;
		}

		leaf_action action = parseAction(buf);
		if (action != ACTION_NONE){
			LOGF("[Hook][apply] Adding action " + std::to_string(action) + " to hook");
			_actions.push_back(action);
		}
	}
}

static leaf_action parseAction(std::string& buf){
	FUN();

	leaf_action action;
	if (buf == "update")
		action = ACTION_UPDATE;
	else if (buf == "install")
		action = ACTION_INSTALL;
	else if (buf == "remove")
		action = ACTION_REMOVE;
	else if (buf == "installLocal")
		action = ACTION_INSTALLLOCAL;
	else
		action = ACTION_NONE;

	return action;
}
