#include "error.h"

std::string errorCodes[] = {
	"Database is not accessible",
	"Leaf root is not accessible",
	"Leaf configuration could not be accessed",
	"Leaf cache could not be accessed",
	
	"Could not create file",
	"Could not create directory",
	"Could not remove file",
	"Could not remove directory",
	"General filesystem error",

	"No error"
};

std::string LeafError::getErrorCodeMessage(){
	return errorCodes[_errorCode];
}