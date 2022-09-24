/**
 * @file		leafcore/getBranchMasterECString.cpp
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		The implementation of Leafcore::getBranchMasterEC()
 * @copyright	Copyright (c) 2022
 */
#include "log.h"
#include "error.h"
#include "leafdebug.h"

#include "leafcore.h"

std::string Leafcore::getBranchMasterECString(BranchMaster::ec error){
	FUN();

	using namespace BranchMaster;

	switch (error){

		case E_REQUEST:
			return "The request was malformed, leaf may need updating.";

		case E_GET:
			return "An invalid get= value was supplied, leaf may need updating.";

		case E_PKGNAME:
			return "The pkgname= tag is missing in the request, leaf may need updating.";

		case E_VERSIONTAG:
			return "The version= tag is missing in the request, leaf may need updating.";

		case E_GENERAL:
			return "The requested information could not be found.";

		case E_PACKAGE:
			return "The package could not be found, wrong name?";

		case E_VERSION:
			return "The specified version is not available, out-of-date package list?";

		case E_NONE:
			return "No error";

		default:
			return "Unresolved error code";
	}
}
