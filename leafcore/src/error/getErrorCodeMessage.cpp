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
	"Could not open file for reading",
	"Could not open file for writing",
	"Could not open directory",
	"Path is not a directory",
	"Could not change to directory",
	"General filesystem error",

	//Package management
	"Failed to parse leafinstalled file",

	//Actions
	"Package was not found",
	"Package is already installed",
	"Package is not installed",
	"Package is not downloaded",
	"Package is not extracted",

	//Archive
	"It seems that an archive has already been loaded",
	"No archive was loaded",
	"Libarchive has warned",
	"Libarchive encountered an error",
	"Libarchive had a fatal failure",

	"Package list not loaded",

	"No error"
};

std::string LeafError::getErrorCodeMessage(){
	return errorCodes[_errorCode];
}