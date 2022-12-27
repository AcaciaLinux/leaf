#include "../t_leafcore.h"

#include <filesystem>
#include <fstream>

TEST(Leafcore, parseConfig){
	FUN();

	LEAF_DEBUG_SET_FAIL("Leafcore::parseConfig()");

	try{
		
		Leafcore core;

		F_NOTHROW(Error::DEBUG_EXCEPTION);
	} catch (LeafError* e){
		CHECK_EC(Error::DEBUG_EXCEPTION, e);
	} catch (...){
		F_WRONGEXCEPTION("LeafError*");
	}
}

//Checks if the chroot-cmd config gets validated correctly
TEST(Leafcore, parseConfig_chroot_check){
	std::filesystem::create_directories("etc/leaf");
	std::ofstream outFile("etc/leaf/leaf.conf");
	outFile << "chroot-cmd=/sbin/chroot {COMMAND} {ROOTDIR}" << std::endl;
	outFile.close();

	leaf_config_t conf;
	conf.setRootDir("./");

	try {
		Leafcore core(conf);
	} catch (LeafError* e){
		FAIL() << "Unexpected leaf error: " << e->what();
	}
}

//Checks if the chroot-cmd config gets validated correctly
TEST(Leafcore, parseConfig_chroot_check_missing_command){
	std::filesystem::create_directories("etc/leaf");

	std::ofstream outFile("etc/leaf/leaf.conf");
	outFile << "chroot-cmd=/sbin/chroot {ROOTDIR}" << std::endl;
	outFile.close();

	leaf_config_t conf;
	conf.setRootDir("./");

	try {
		Leafcore core(conf);
		F_NOTHROW(Error::CONFF_INV_CONF);
	} catch (LeafError* e){
		ASSERT_EQ(Error::CONFF_INV_CONF, e->getErrorCode()) << "Wrong exception thrown, expected CONFF_INV_CONF, got " + e->what();
	}
}

//Checks if the chroot-cmd config gets validated correctly
TEST(Leafcore, parseConfig_chroot_check_missing_rootdir){
	std::filesystem::create_directories("etc/leaf");

	std::ofstream outFile("etc/leaf/leaf.conf");
	outFile << "chroot-cmd=/sbin/chroot {COMMAND}" << std::endl;
	outFile.close();

	leaf_config_t conf;
	conf.setRootDir("./");

	try {
		Leafcore core(conf);
		F_NOTHROW(Error::CONFF_INV_CONF);
	} catch (LeafError* e){
		ASSERT_EQ(Error::CONFF_INV_CONF, e->getErrorCode()) << "Wrong exception thrown, expected CONFF_INV_CONF, got " + e->what();
	}
}
