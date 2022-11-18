#ifndef __BRANCHMASTER_H__
#define __BRANCHMASTER_H__

#include <string>

namespace BranchMaster{
	enum ec{
		E_NONE,
		
		E_REQUEST,
		E_GET,
		E_PKGNAME,
		E_VERSIONTAG,
		E_GENERAL,
		E_PACKAGE,
		E_VERSION
	};

	/**
	 * @brief	Parses the branchmaster error code responded via the http body
	 * @param	httpCode		The HTTP response code
	 * @param	response		The response
	 */
	BranchMaster::ec 			parseEC(size_t httpCode, const std::string& response);

	/**
	 * @brief	Return a full string description of a BranchMaster error code
	 * @param	ec				The error code to resolve
	 */
	std::string					getECString(BranchMaster::ec ec);
}

#endif
