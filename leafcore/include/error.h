#ifndef __ERROR_H__
#define __ERROR_H__

#include <string>
#include <system_error>

//	1x		General leaf errors
//	2x		User answers
//	3x		FS - general
//	4x		FS - files
//	5x		FS - directories
//	6x		Actions
//	7x		Archive
//	8x		Downloader
//	9x		Package
//	10x		PackageListParser
//	11x		Hooks
//	12x		JSON
//	13x		BranchMaster
//	14x		LeafFS
//	15x		LeafDB
//	16x		Debugging
//	17x		Unimplemented feature
//	18x		None

namespace Error{
	
	enum ec{
		
		//General leaf errors
		NODB = 10,
		NOROOT = 11,
		NOCONFIG = 12,
		NOCACHE = 13,
		NOPACKAGE = 14,
		NULLPARAMETER = 15,

		//User answers
		USER_DISAGREE = 20,

		//FS - general
		FS_ERROR = 30,
		BAD_ISTREAM = 31,
		BAD_OSTREAM = 32,
		REMOVE = 33,
		
		//FS - files
		CREATEFILE = 40,
		COPYFILE = 41,
		REMOVEFILE = 42,
		OPENFILER = 43,
		OPENFILEW = 44,

		//FS - directories
		CREATEDIR = 50,
		REMOVEDIR = 51,
		OPENDIR = 52,
		NOTDIR = 53,
		CHDIR = 54,

		//Actions
		PKG_NOTFOUND = 60,
		PKG_INSTALLED = 61,
		PKG_NOTINSTALLED = 62,
		PKG_NOTDOWNLOADED = 63,
		PKG_NOTEXTRACTED = 64,
		PKG_NOTLFPKG = 65,
		PKG_NODELIMITER = 66,
		PKG_NOTEXISTING = 67,
		PKGLIST_NOTLOADED = 68,

		//Archive
		ARCH_ALREADYOPEN = 70,
		ARCH_NOTLOADED = 71,
		ARCH_COPY = 72,
		ARCH_WARN = 73,
		ARCH_ERR = 74,
		ARCH_FATAL = 75,

		//Downloader
		DL_INIT = 80,
		DL_NOT_INIT = 81,
		DL_BAD_STREAM = 82,
		DL_CURL_ERR = 83,

		//Package
		PACKAGE_UNEXPECTED_EOF = 90,
		PACKAGE_STOI = 91,
		PACKAGE_FILE_EXISTS = 92,
		PACKAGE_FETCH_DEST_EMPTY = 93,
		PACKAGE_SCRIPT_FAILED = 94,
		PACKAGE_PARSE_INSTALLEDFILE = 95,

		//PackageListParser
		PKGPRS_BAD_STREAM = 101,
		PKGPRS_APPLY_DB = 102,
		PKGPRS_LIST_N_SUCCESS = 103,

		//Hooks
		HOOK_REQUIRED_VALUE = 110,

		//JSON
		JSON_OUT_OF_RANGE = 120,

		//BranchMaster
		BRANCHMASTER_ERROR = 130,

		//LeafFS
		LEAFFS_DIR_STR_EMPTY = 140,
		LEAFFS_DIR_NOT_EXISTING = 141,
		LEAFFS_DIR_NOT_DIR = 142,
		LEAFFS_FILE_NOT_EXISTING = 143,

		//LeafDB
		LEAFDB_PKG_DEP_NOTFOUND = 150,
		LEAFDB_PKG_NOT_FOUND = 151,

		//Debugging exception
		DEBUG_EXCEPTION = 160,
		
		//Unimplemented feature
		FEATURE_NOT_IMPLEMENTED = 170,

		NONE = 180
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