#include "error.h"

std::string errorCodes[] = {

	//General leaf errors
	"Database is not accessible",
	"Leaf root is not accessible",
	"Leaf configuration could not be accessed",
	"Leaf cache could not be accessed",

	//User answers
	"User disagreed",

	//File system
	"Could not create file",
	"Could not create directory",
	"Could not remove file",
	"Could not remove directory",
	"Path is not a directory",
	"General filesystem error",

	//Package management
	"Failed to parse leafinstalled file",

	//Actions
	"Package is already installed",
	"Package is not installed",
	"Package is not downloaded",
	"Package is not extracted",

	"No error"
};

std::string LeafError::getErrorCodeMessage(){
	return errorCodes[_errorCode];
}