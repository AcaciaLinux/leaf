#include <gtest/gtest.h>
#include <stdlib.h>

#define FRIEND_PACKAGE
#include "package.h"

#include "random.h"
#include "log.h"

TEST(Package, Constructor){
	FUN();

	std::string name;
	uint8_t len = random_i(0, INT8_MAX);
	name.reserve(len);

	for (uint8_t i = 0; i < len; i++){
		name += (char)(random_i('0', 'z'));
	}

	LOGD("Generated random name with length " + std::to_string(len) + ": \"" + name + "\"");
	
	Package package(name);

	if (package._name != name)
		FAIL();
}