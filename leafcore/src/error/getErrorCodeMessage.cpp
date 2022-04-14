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
	"Bad input stream",
	"Bad output stream",

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
	"Archive copy failed",
	"Libarchive has warned",
	"Libarchive encountered an error",
	"Libarchive had a fatal failure",

	//Download
	"Libcurl failed to initialize",
	"Downloader was not initialized",
	"Downloader has encountered a bad stream",
	"Libcurl had an error",

	//Package
	"Unexpected EOF when parsing",
	"Failed to convert string to integer",

	//Package list
	"Package list not loaded",

	//Package database
	"Package dependency not found",

	//Debugging exception
	"Debugging exception at function",

	"No error"
};

std::string LeafError::getErrorCodeMessage(){
	return errorCodes[_errorCode];
}

std::string LeafError::errorCode(Error::ec e){
	return errorCodes[e];
}
