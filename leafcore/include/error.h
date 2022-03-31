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

		//User answers
		USER_DISAGREE,
		
		//File system
		CREATEFILE,
		CREATEDIR,
		REMOVEFILE,
		REMOVEDIR,
		OPENFILER,
		OPENFILEW,
		OPENDIR,
		NOTDIR,
		CHDIR,
		FS_ERROR,

		//Package management
		PARSE_INSTALLEDFILE,

		//Actions
		PKG_NOTFOUND,
		PKG_INSTALLED,
		PKG_NOTINSTALLED,
		PKG_NOTDOWNLOADED,
		PKG_NOTEXTRACTED,

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

		//PackageListParse
		PKGPRS_BAD_STREAM,
		PKGPRS_APPLY_DB,

		//Package list
		PKGLIST_NOTLOADED,

		//Debugging exception
		DEBUG_EXCEPTION,

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