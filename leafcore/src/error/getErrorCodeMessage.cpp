#include "error.h"

std::string LeafError::getErrorCodeMessage() const{
	return errorCodeToString(_errorCode);
}

std::string LeafError::errorCodeToString(Error::ec e){
	using namespace Error;

	switch(e){

		//1x General leaf errors
		case NODB:
			return "Database is not accessible";
		case NOROOT:
			return "Leaf root is not accessible";
		case NOCONFIG:
			return "Leaf configuration could not be accessed";
		case NOCACHE:
			return "Leaf cache could not be accessed";
		case NOPACKAGE:
			return "No packages have been supplied for processing";
		case NULLPARAMETER:
			return "An argument has been a nullptr (THIS IS A BUG!!!)";
		case ABORT:
			return "The current action was aborted";

		//2x User answers
		case USER_DISAGREE:
			return "User disagreed";

		//3x FS - general
		case FS_ERROR:
			return "General filesystem error";
		case BAD_ISTREAM:
			return "Bad input stream";
		case BAD_OSTREAM:
			return "Bad output stream";
		case REMOVE:
			return "Could not remove filesystem entry";
		case FS_EXISTS:
			return "Could not check if fs entry exists";
		case FS_GETWDIR:
			return "Could not get the current workdirectory";
		case FS_CHWDIR:
			return "Could not change the working directory";
		case FS_CHECK_TYPE:
			return "Could not check type of filesystem entry";

		//4x FS - files
		case CREATEFILE:
			return "Could not create file";
		case COPYFILE:
			return "Could not copy file";
		case REMOVEFILE:
			return "Could not remove file";
		case OPENFILER:
			return "Could not open file for reading";
		case OPENFILEW:
			return "Could not open file for writing";

		//5x FS - directories
		case CREATEDIR:
			return "Could not create directory";
		case REMOVEDIR:
			return "Could not remove directory";
		case OPENDIR:
			return "Could not open directory";
		case NOTDIR:
			return "Path is not a directory";
		case CHDIR:
			return "Could not change to directory";
		case MKDIR:
			return "Could not create directory";

		//6x Actions
		case PKG_NOTFOUND:
			return "Package was not found";
		case PKG_INSTALLED:
			return "Package is already installed";
		case PKG_NOTINSTALLED:
			return "Package is not installed";
		case PKG_NOTDOWNLOADED:
			return "Package is not downloaded";
		case PKG_NOTEXTRACTED:
			return "Package is not extracted";
		case PKG_NOTLFPKG:
			return "Package is not a .lfpkg file";
		case PKG_NODELIMITER:
			return "Package name does not contain the '-' delimiter for the version";
		case PKG_NOTEXISTING:
			return "Local package archive does not exist";
		case PKGLIST_NOTLOADED:
			return "Package list not loaded";

		//7x Archive
		case ARCH_ALREADYOPEN:
			return "It seems that an archive has already been loaded";
		case ARCH_NOTLOADED:
			return "No archive was loaded";
		case ARCH_COPY:
			return "Archive copy failed";
		case ARCH_WARN:
			return "Libarchive has warned";
		case ARCH_ERR:
			return "Libarchive encountered an error";
		case ARCH_FATAL:
			return "Libarchive had a fatal failure";

		//8x Downloader
		case DL_INIT:
			return "Libcurl failed to initialize";
		case DL_NOT_INIT:
			return "Downloader was not initialized";
		case DL_BAD_STREAM:
			return "Downloader has encountered a bad stream";
		case DL_CURL_ERR:
			return "Libcurl had an error";
		case DL_BAD_RESPONSE:
			return "Downloader has received a bad response code";

		//9x Package
		case PACKAGE_UNEXPECTED_EOF:
			return "Unexpected EOF when parsing";
		case PACKAGE_STOI:
			return "Failed to convert string to integer";
		case PACKAGE_FILE_EXISTS:
			return "File does already exist";
		case PACKAGE_FETCH_DEST_EMPTY:
			return "Package download destination is empty";
		case PACKAGE_SCRIPT_FAILED:
			return "Package script failed to execute";
		case PACKAGE_PARSE_INSTALLEDFILE:
			return "Failed to parse leafinstalled file";

		//10x PackageListParser
		case PKGPRS_BAD_STREAM:
			return "Package list parser: Bad stream";
		case PKGPRS_APPLY_DB:
			return "Package list parser: Apply to database";
		case PKGPRS_LIST_N_SUCCESS:
			return "Package list parser: Package list fetch not successfull";

		//11x Hooks
		case HOOK_REQUIRED_VALUE:
			return "Hook: Could not find required value";

		//12x JSON
		case JSON_OUT_OF_RANGE:
			return "JSON: Out of range";
		case JSON_PARSE:
			return "JSON: Failed to parse";

		//13x BranchMaster
		case BRANCHMASTER_ERROR:
			return "Package server error";

		//14x LeafFS
		case LEAFFS_DIR_STR_EMPTY:
			return "LeafFS: Directory string is empty";
		case LEAFFS_DIR_NOT_EXISTING:
			return"LeafFS: Directory does not exist";
		case LEAFFS_DIR_NOT_DIR:
			return "LeafFS: Specified path is not a directory";
		case LEAFFS_FILE_NOT_EXISTING:
			return "LeafFS: File does not exist";

		//15x LeafDB
		case LEAFDB_PKG_DEP_NOTFOUND:
			return "LeafDB: Package dependency not found";
		case LEAFDB_PKG_NOT_FOUND:
			return "LeafDB: Package not found";

		//16x Config file
		case CONFF_INV_CONF:
			return "Config file: Invalid configuration";

		//17x Debugging exception
		case DEBUG_EXCEPTION:
			return "Debugging exception at function";

		//18x Unimplemented feature
		case FEATURE_NOT_IMPLEMENTED:
			return "Feature is currently not implemented";

		//19x NONE
		case NONE:
			return "No error";

		default:
			return "Undefined error code " + std::to_string(e);

	}
}
