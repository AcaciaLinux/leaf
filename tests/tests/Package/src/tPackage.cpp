#include <gtest/gtest.h>
#include <stdlib.h>

#define FRIEND_PACKAGE
#include "package.h"

#include "random.h" 
#include "log.h"

TEST(Package, Constructor){
	FUN();

	std::string name;
	uint8_t len = random_i(0, UINT8_MAX);
	name.reserve(len);

	for (uint8_t i = 0; i < len; i++){
		name += (char)(random_i('0', 'z'));
	}

	LOGD("Generated random name with length " + std::to_string(len) + ": \"" + name + "\"");
	
	Package package(name, "0.0");

	if (package._name != name)
		FAIL();
}

TEST(Package, addProvidedFile){
	FUN();

	std::string filename;
	uint8_t len = random_i(0, UINT8_MAX);
	filename.reserve(len);

	for (uint8_t i = 0; i < len; i++){
		filename += (char)(random_i('0', 'z'));
	}

	LOGD("Generated random filename with length " + std::to_string(len) + ": \"" + filename + "\"");

	Package package("Testpackage", "0.0");
	package.addProvidedFile(filename);

	if (package._provided_files.back() != filename)
		FAIL();
}

TEST(Package, checkFileProvided){
	FUN();

	std::string filename;
	uint8_t len = random_i(0, UINT8_MAX);
	filename.reserve(len);

	for (uint8_t i = 0; i < len; i++){
		filename += (char)(random_i('0', 'z'));
	}

	LOGD("Generated random filename with length " + std::to_string(len) + ": \"" + filename + "\"");

	Package package("Testpackage", "0.0");
	package.addProvidedFile(filename);

	//Fail if Package finds a file that is not contained
	if (package.checkFileProvided(filename + "!"))
		FAIL();

	//Fail if Package does not find a contained file
	if (!package.checkFileProvided(filename))
		FAIL();

}