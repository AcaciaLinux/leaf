#include "../t_leafArchive.h"
#include "testing_fs.h"

#include <fstream>

TEST(LeafArchive, extract_debug){
	FUN();

	LEAF_DEBUG_SET_FAIL("LeafArchive::extract()");

	try{
		
		LeafArchive ar;
		ar.extract("noDirectory");

		F_NOTHROW(Error::DEBUG_EXCEPTION);
	} catch (LeafError* e){
		CHECK_EC(Error::DEBUG_EXCEPTION, e);
	} catch (...){
		F_WRONGEXCEPTION("LeafError*");
	}
}

TEST(LeafArchive, extract_notLoaded){
	FUN();

	try{
		
		LeafArchive ar;
		ar.extract("noDirectory");

		F_NOTHROW(Error::ARCH_NOTLOADED);
	} catch (LeafError* e){
		CHECK_EC(Error::ARCH_NOTLOADED, e);
	} catch (...){
		F_WRONGEXCEPTION("LeafError*");
	}
}

/**
 * @brief	Tests the handling of a non-existing destination path
 */
TEST(LeafArchive, extract_noDestDir){
	FUN();

	try{
		
		LeafArchive ar;
		ar._archive = &ar;
		ar._ext = &ar;
		ar.extract("ThisPathShouldNeverExistAnyWhere.extension.random");

		F_NOTHROW(Error::OPENDIR);
	} catch (LeafError* e){
		CHECK_EC(Error::OPENDIR, e);
	} catch (...){
		F_WRONGEXCEPTION("LeafError*");
	}
}

/**
 * @brief	Tests the handling of the destination not being a directory
 */
TEST(LeafArchive, extract_destNotDir){
	FUN();

	std::string outFileName = "downloadTestFile.file.test";

	try{
		createFile(outFileName);
	} catch (FSError* e){
		F_THROW(e);
	}

	try{
		
		LeafArchive ar;
		ar._archive = &ar;
		ar._ext = &ar;
		ar.extract(outFileName);

		F_NOTHROW(Error::NOTDIR);
	} catch (LeafError* e){
		CHECK_EC(Error::NOTDIR, e);
	} catch (...){
		F_WRONGEXCEPTION("LeafError*");
	}

	try{
		deletePath(outFileName);
	} catch (FSError* e){
		F_THROW(e);
	}
}
