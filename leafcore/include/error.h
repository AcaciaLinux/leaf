#ifndef __ERROR_H__
#define __ERROR_H__

#include <string>

namespace Error{
	
	enum ec{
		
		//General leaf errors
		NODB,
		NOROOT,
		NOCONFIG,
		NOCACHE,
		
		CREATEFILE,
		CREATEDIR,
		REMOVEFILE,
		REMOVEDIR,
		FS_ERROR,

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

	std::string getErrorCodeMessage();

	std::string what(){
		if (_additional.empty())
			return getErrorCodeMessage();
		else
			return getErrorCodeMessage() + _additional;
	}

	Error::ec getErrorCode(){
		return _errorCode;
	}

private:
	Error::ec			_errorCode;
	std::string			_additional;
};

#endif