#ifndef __BRANCHMASTER_H__
#define __BRANCHMASTER_H__

#include <string>

namespace BranchMaster{
	enum ec{
		E_NONE,
		
		E_REQUEST,
		E_GET,
		E_PKGNAME,
		E_VERSION,
		E_GENERAL,
		E_PACKAGE
	};

	/**
	 * @brief	Parses the branchmaster error code responded via the http body
	 * @param	response		The response
	 */
	BranchMaster::ec parseEC(const std::string& response);
}

#endif
