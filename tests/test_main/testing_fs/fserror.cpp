#include "log.h"
#include "testing_fs.h"

FSError::FSError(std::string message) : _message(message){
	FUN();
}

FSError::FSError(std::string message, std::error_code ec) : _message(message), _ec(ec){
	FUN();
}

std::string FSError::what(){
	if (_ec)
		return _message + ": " + _ec.message();
	else
		return _message;
}
