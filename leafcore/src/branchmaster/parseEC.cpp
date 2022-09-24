/**
 * @file		branchmaster/parseEC.cpp
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		The implementation of BranchMaster::parseEC()
 * @copyright	Copyright (c) 2022
 */
#include "log.h"
#include "error.h"
#include "leafdebug.h"

#include "branchmaster.h"

#include <regex>

//TODO: Tests

BranchMaster::ec BranchMaster::parseEC(const std::string& str){
	FUN();
	LEAF_DEBUG_EX("BranchMaster::parseEC()");

	if (std::regex_match(str, std::regex(".*E_REQUEST.*")))
		return BranchMaster::E_REQUEST;
	else if (std::regex_match(str, std::regex(".*E_GET.*")))
		return BranchMaster::E_GET;
	else if (std::regex_match(str, std::regex(".*E_PKGNAME.*")))
		return BranchMaster::E_PKGNAME;
	else if (std::regex_match(str, std::regex(".*E_VERSION.*")))
		return BranchMaster::E_VERSION;
	else if (std::regex_match(str, std::regex(".*E_GENERAL.*")))
		return BranchMaster::E_GENERAL;
	else if (std::regex_match(str, std::regex(".*E_PACKAGE.*")))
		return BranchMaster::E_PACKAGE;
	
	return BranchMaster::E_NONE;
}
