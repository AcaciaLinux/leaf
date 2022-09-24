#ifndef __ERROR_H__
#define __ERROR_H__

#include <string>
#include <system_error>

namespace Error{
	
	enum ec{
		
		//General leaf errors
		NODB,
		NOROOT,
		NOCONFIG,
		NOCACHE,
		NOPACKAGE,
		NULLPARAMETER,

		//User answers
		USER_DISAGREE,
		
		//File system
		CREATEFILE,
		CREATEDIR,
		COPYFILE,
		REMOVE,
		REMOVEFILE,
		REMOVEDIR,
		OPENFILER,
		OPENFILEW,
		OPENDIR,
		NOTDIR,
		CHDIR,
		FS_ERROR,
		BAD_ISTREAM,
		BAD_OSTREAM,

		//Package management
		PARSE_INSTALLEDFILE,

		//Actions
		PKG_NOTFOUND,
		PKG_INSTALLED,
		PKG_NOTINSTALLED,
		PKG_NOTDOWNLOADED,
		PKG_NOTEXTRACTED,
		PKG_NOTLFPKG,
		PKG_NODELIMITER,
		PKG_NOTEXISTING,

		//Archive
		ARCH_ALREADYOPEN,
		ARCH_NOTLOADED,
		ARCH_COPY,
		ARCH_WARN,
		ARCH_ERR,
		ARCH_FATAL,

		//Downloader
		DL_INIT,
		DL_NOT_INIT,
		DL_BAD_STREAM,
		DL_CURL_ERR,

		//Package
		PACKAGE_UNEXPECTED_EOF,
		PACKAGE_STOI,
		PACKAGE_FILE_EXISTS,
		PACKAGE_FETCH_DEST_EMPTY,
		PACKAGE_SCRIPT_FAILED,

		//PackageListParser
		PKGPRS_BAD_STREAM,
		PKGPRS_APPLY_DB,

		//Hooks
		HOOK_REQUIRED_VALUE,

		//Package list
		PKGLIST_NOTLOADED,

		//BranchMaster
		BRANCHMASTER_ERROR,

		//LeafFS
		LEAFFS_DIR_STR_EMPTY,
		LEAFFS_DIR_NOT_EXISTING,
		LEAFFS_DIR_NOT_DIR,
		LEAFFS_FILE_NOT_EXISTING,

		//LeafDB
		LEAFDB_PKG_DEP_NOTFOUND,
		LEAFDB_PKG_NOT_FOUND,

		//Debugging exception
		DEBUG_EXCEPTION,
		
		//Unimplemented feature
		FEATURE_NOT_IMPLEMENTED,

		NONE		
	};

}

class LeafError{

public:
	LeafError(Error::ec errorCode){
		_errorCode = errorCode;
	}

	LeafError(Error::ec errorCode, std::string additional){
		_errorCode = errorCode;
		_additional = additional;
	}

	LeafError(Error::ec errorCode, std::error_code stdErrorCode){
		_errorCode = errorCode;
		_stdErrorCode = stdErrorCode;
	}

	LeafError(Error::ec errorCode, std::string additional, std::error_code stdErrorCode){
		_errorCode = errorCode;
		_additional = additional;
		_stdErrorCode = stdErrorCode;
	}

	std::string getErrorCodeMessage();

	static std::string errorCode(Error::ec e);

	std::string what(){
		std::string ret = getErrorCodeMessage();

		if (!_additional.empty())
			ret += ": " + _additional;

		if (_stdErrorCode)
			ret += ": " + _stdErrorCode.message();

		return ret;
	}

	Error::ec getErrorCode(){
		return _errorCode;
	}

private:
	Error::ec			_errorCode;
	std::string			_additional;
	std::error_code		_stdErrorCode;
};

#endif