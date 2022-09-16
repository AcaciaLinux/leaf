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
#include <deque>

static std::deque<std::string> splitBy(std::string& str, char separator);
static leaf_action parseAction(std::string& str);
static hook_exec_time parseExecTime(std::string& str);

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

	{//Parse all the actions
		std::deque<std::string> actions = splitBy(entries["action"], ',');

		for (std::string actionString : actions){
			leaf_action action = parseAction(actionString);
			LOGF("[Hook][apply] Adding action " + std::to_string(action) + " to hook");
			_actions.push_back(action);
		}
	}

	{//Parse all the packages
		auto packages = splitBy(entries["packages"], ',');

		for (std::string packageString : packages){
			LOGF("[Hook][apply] Adding package " + packageString + " to hook");
			_packages.push_back(packageString);
		}
	}

	_execTime = parseExecTime(entries["when"]);
	#warning Rundependencies
	_exec = entries["exec"];
}

static std::deque<std::string> splitBy(std::string& str, char separator){
	FUN();

	std::deque<std::string> res;

	std::string buf;
	for (char c : str){
		if (c == separator){
			res.push_back(std::string(buf));
			buf.clear();
		} else
			buf += c;
	}
	res.push_back(std::string(buf));

	return res;
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

static hook_exec_time parseExecTime(std::string& str){
	FUN();

	hook_exec_time time = HOOK_EXEC_NEVER;

	if (str == "pre")
		time = HOOK_EXEC_PRE;
	else if (str == "post")
		time = HOOK_EXEC_POST;

	return time;
}
