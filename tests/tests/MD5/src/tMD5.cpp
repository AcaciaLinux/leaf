#include "../t_md5.h"

#include <fstream>
#include <filesystem>

TEST(MD5, MD5_hash){
	FUN();

	std::ofstream outFile("test.txt");
	outFile << "This is a test string" << std::endl;
	outFile.close();

	std::ifstream inFile("test.txt");

	ASSERT_EQ("b584c39f97dfe71ebceea3fdb860ed6c", md5(inFile)) << "Hash of example file does not match";

	inFile.close();

	std::filesystem::remove("test.txt");
}
