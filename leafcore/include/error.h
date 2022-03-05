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
		FS_ERROR,

		//Package management
		PARSE_INSTALLEDFILE,

		//Actions
		PKG_NOTFOUND,
		PKG_INSTALLED,
		PKG_NOTINSTALLED,
		PKG_NOTDOWNLOADED,
		PKG_NOTEXTRACTED,

		PKGLIST_NOTLOADED,

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