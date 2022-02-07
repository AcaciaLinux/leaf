
#include <iostream>
#include <string>

#include <fstream>

#include "log.h"
#include "leafdb.h"
#include "arguments.h"

#include "downloader.h"
#include "pkglistparser.h"
#include "leafcore.h"

Arguments arguments;

Log::Log* hlog;

int main(int argc, char** argv){
	hlog = new Log::Log(Log::U);
	hlog->setFeature(Log::FEATURE_PRINTFUNNAMES, false);
	FUN();

	if (!arguments.parse(argc, argv)){
		return -1;
	}

	if (arguments.getAction() == ACTION_UPDATE){

		Leafcore leaf(".");
		leaf.a_update();

		return 0;
	} else if (arguments.getAction() == ACTION_INSTALL) {
		if (arguments.getPackages().size() == 0){
			LOGUE("Specify at least one package!");
			return -1;
		}

		Leafcore leaf(".");
		if (!leaf.parsePackageList()){
			LOGUE("Leaf core parser error: " + leaf.getError());
			return -1;
		}

		std::ifstream inFile;
		inFile.open("packages.list", std::ios::in);

		if (!inFile.is_open()){
			LOGUE("Could not open packages.list!");
			inFile.close();
			return -1;
		}

		PackageListParser parser;
		parser.parse(inFile);

		inFile.close();

		LeafDB db;
		if (!parser.applyToDB(db)){
			LOGUE("Failed to apply to database: " + parser.getError());
		}

		Package* available = db.getPackage(arguments.getPackages().at(0));
		if (available == nullptr){
			LOGUE("Could not find package in database!");
			return -1;
		}
		
		auto dependencies = db.resolveDependencies(available);
		if (dependencies.size() == 0 && !db.getError().empty()){
			LOGUE("Resolving dependencies failed: " + db.getError());
		} else {
			LOGU("Dependencies for " + available->getName() + ":");
			for (Package* p : dependencies){
				LOGU(" -> " + p->getName());
			}
		}
	}

	{
		std::string out = "Packages to process:";
		for (std::string i : arguments.getPackages())
			out += " " + i;
		LOGU(out);
	}
}
