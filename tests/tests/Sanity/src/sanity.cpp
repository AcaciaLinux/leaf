#include <gtest/gtest.h>

#include "t_common.h"
#include "leafconfig.h"

TEST(Sanity, Sanity){
	ASSERT_TRUE(true);
}

//Tests if the rootDir in the config struct is set correctly when testing
TEST(Sanity, debug_rootdir){
	FUN();

	leaf_config_t c;
	ASSERT_EQ(LEAF_TESTING_ROOTDIR, c.rootDir) << "RootDir does not get changed for tests! Tests could write to your system's root!";
}
