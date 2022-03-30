#include "../t_leafArchive.h"
#include "random.h"

#include <fstream>

TEST(LeafArchive, load_debug){
	FUN();

	LEAF_DEBUG_SET_FAIL("LeafArchive::load()");

	try{
		
		LeafArchive ar;
		ar.load("noPath");

		F_NOTHROW(Error::DEBUG_EXCEPTION);
	} catch (LeafError* e){
		CHECK_EC(Error::DEBUG_EXCEPTION, e);
	} catch (...){
		F_WRONGEXCEPTION("LeafError*");
	}
}

TEST(LeafArchive, load_archive_alreadyopen){
	FUN();

	try{
		
		LeafArchive ar;
		ar._archive = &ar;
		ar.load("noPath");

		F_NOTHROW(Error::ARCH_ALREADYOPEN);
	} catch (LeafError* e){
		CHECK_EC(Error::ARCH_ALREADYOPEN, e);
	} catch (...){
		F_WRONGEXCEPTION("LeafError*");
	}
}

TEST(LeafArchive, load_ext_alreadyopen){
	FUN();

	try{
		
		LeafArchive ar;
		ar._ext = &ar;
		ar.load("noPath");

		F_NOTHROW(Error::ARCH_ALREADYOPEN);
	} catch (LeafError* e){
		CHECK_EC(Error::ARCH_ALREADYOPEN, e);
	} catch (...){
		F_WRONGEXCEPTION("LeafError*");
	}
}

TEST(LeafArchive, load_archive_ext_alreadyopen){
	FUN();

	try{
		
		LeafArchive ar;
		ar._ext = &ar;
		ar._archive = &ar;
		ar.load("noPath");

		F_NOTHROW(Error::ARCH_ALREADYOPEN);
	} catch (LeafError* e){
		CHECK_EC(Error::ARCH_ALREADYOPEN, e);
	} catch (...){
		F_WRONGEXCEPTION("LeafError*");
	}
}

/**
 * @brief	Tests the handling of loading a non existing file
 */
TEST(LeafArchive, load_openPath){
	FUN();

	try{
		
		LeafArchive ar;
		
		ar.load("ThisPathShouldNeverExistAnyWhere.extension.random");

		F_NOTHROW(Error::OPENFILER);
	} catch (LeafError* e){
		CHECK_EC(Error::OPENFILER, e);
	} catch (...){
		F_WRONGEXCEPTION("LeafError*");
	}
}
