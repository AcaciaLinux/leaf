#include "error.h"

std::string errorCodes[] = {

	//General leaf errors
	"Database is not accessible",
	"Leaf root is not accessible",
	"Leaf configuration could not be accessed",
	"Leaf cache could not be accessed",
	"No packages have been supplied for processing",
	"An argument has been a nullptr (THIS IS A BUG!!!)",

	//User answers
	"User disagreed",

	//File system
	"Could not create file",
	"Could not create directory",
	"Could not copy file",
	"Could not remove filesystem entry",
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
	"File does already exist",
	"Package download destination is empty",
	"Package script failed to execute",

	//PackageListParser
	"Package list parser: Bad stream",
	"Package list parser: Apply to database",

	//Package list
	"Package list not loaded",

	//LeafFS
	"LeafFS: Directory string is empty",
	"LeafFS: Directory does not exist",
	"LeafFS: Specified path is not a directory",
	"LeafFS: File does not exist",

	//LeafDB
	"LeafDB: Package dependency not found",
	"LeafDB: Package not found",

	//Debugging exception
	"Debugging exception at function",

	//Unimplemented feature
	"Feature is currently not implemented",

	"No error"
};

std::string LeafError::getErrorCodeMessage(){
	return errorCodes[_errorCode];
}

std::string LeafError::errorCode(Error::ec e){
	return errorCodes[e];
}
