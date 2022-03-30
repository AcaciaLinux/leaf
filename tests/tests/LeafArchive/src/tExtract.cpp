#include "../t_leafArchive.h"

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

	{//Create the destination as a file
		std::error_code ec;
	
		bool exists = std::filesystem::exists(outFileName, ec);

		if (ec)
			FAIL() << "Failed to check if file " << outFileName << " exists";
		
		if (exists){
			std::filesystem::remove_all(outFileName, ec);
			if (ec)
				FAIL() << "Failed to delete existing file " << outFileName << " for testing";
		}

		std::ofstream newFile;
		newFile.open(outFileName, std::ios::out);

		if (!newFile.is_open())
			FAIL() << "Failed to create file " << outFileName << " for testing";

		newFile.close();
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

	{	//Clean up the file
		std::error_code ec;
	
		bool exists = std::filesystem::exists(outFileName, ec);

		if (ec)
			FAIL() << "Failed to check if file " << outFileName << " exists";
		
		if (exists){
			std::filesystem::remove_all(outFileName, ec);
			if (ec)
				FAIL() << "Failed to delete existing file " << outFileName << " for testing";
		} else {
			FAIL() << "Operation removed file " << outFileName << " not by design";
		}
	}
}
