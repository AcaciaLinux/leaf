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

#include "pkglistparser.h"

#include <algorithm>
#include <deque>
#include <regex>

static std::deque<std::string> splitBy(const std::string& str, char separator);
static leaf_action parseAction(const std::string& str);
static hook_exec_time parseExecTime(const std::string& str);

void Hook::apply(){
	FUN();
	LEAF_DEBUG_EX("Hook::apply()");

	if (Parser::get("action", "") == "")
		throw new LeafError(Error::HOOK_REQUIRED_VALUE, "'action' in " + _filePath);

	if (Parser::get("package", "") == "")
		throw new LeafError(Error::HOOK_REQUIRED_VALUE, "'packages' in " + _filePath);

	if (Parser::get("when", "") == "")
		throw new LeafError(Error::HOOK_REQUIRED_VALUE, "'when' in " + _filePath);

	if (Parser::get("rundeps", "") == "")
		throw new LeafError(Error::HOOK_REQUIRED_VALUE, "'rundeps' in " + _filePath);

	if (Parser::get("exec", "") == "")
		throw new LeafError(Error::HOOK_REQUIRED_VALUE, "'exec' in " + _filePath);

	{//Parse all the actions
		std::deque<std::string> actions = splitBy(Parser::get("action", ""), ',');

		for (std::string actionString : actions){
			leaf_action action = parseAction(actionString);
			LOGF("[Hook][apply] Adding action " + std::to_string(action) + " to hook");
			_actions.push_back(action);
		}
	}

	{//Parse all the packages
		auto packages = splitBy(Parser::get("packages", ""), ',');

		for (std::string packageString : packages){
			LOGF("[Hook][apply] Adding package " + packageString + " to hook");
			_packages.push_back(packageString);
		}
	}

	_execTime = parseExecTime(Parser::get("when", ""));
	_runDeps = PackageListParser::parseDependenciesString(Parser::get("rundeps", ""));
	_exec = Parser::get("exec", "");
}

static std::deque<std::string> splitBy(const std::string& str, char separator){
	FUN();

	std::deque<std::string> res;

	std::string buf;
	for (char c : str){
		if (c == separator){
			//Remove leading and trailing whitespaces
			buf = std::regex_replace(buf, std::regex("^ +| +$|( ) +"), "$1");

			res.push_back(std::string(buf));
			buf.clear();
		} else
			buf += c;
	}
	//Remove leading and trailing whitespaces
	buf = std::regex_replace(buf, std::regex("^ +| +$|( ) +"), "$1");
	res.push_back(std::string(buf));

	return res;
}

static leaf_action parseAction(const std::string& buf){
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

static hook_exec_time parseExecTime(const std::string& str){
	FUN();

	hook_exec_time time = HOOK_EXEC_NEVER;

	if (str == "pre")
		time = HOOK_EXEC_PRE;
	else if (str == "post")
		time = HOOK_EXEC_POST;

	return time;
}
