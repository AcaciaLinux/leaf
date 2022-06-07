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
		std::string action = entries["action"];
		action.erase(std::remove_if(action.begin(), action.begin(), isspace), action.begin());

		std::string buf;
		for (char c : action){
			if (c == ','){
				#warning Implementation missing: parsing action
				buf.clear();
				continue;
			}

			buf += c;
		}
	}

}
