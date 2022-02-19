#include "../t_package.h"

#include <filesystem>

TEST(Package, noDB){
	FUN();

	Package p("Test", "1.0");

	if (p.fetch())
		FAIL();
}

TEST(Package, noURL){

	std::filesystem::create_directories("./root/etc/leaf");
	std::filesystem::create_directories("./root/var/cache/leaf");
	LeafDB db(new Leafcore("./root"));

	Package* newP = db.newPackage("Test", "1.0");
	
	if (newP->fetch())
		FAIL();
}
