
#include <iostream>
#include <string>

#include <fstream>

#include "log.h"
#include "leafdb.h"
#include "arguments.h"

#include "downloader.h"
#include "pkglistparser.h"

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
		Downloader dl;
		dl.init();

		std::ofstream outFile;
		outFile.open("packages.list", std::ios::out);

		if (!dl.download("https://raw.githubusercontent.com/AcaciaLinux/leaf_packages/main/packages.list", outFile)){
			LOGUE("Error: " + dl.getError());
		}

		outFile.close();
		return 0;
	} else if (arguments.getAction() == ACTION_INSTALL) {
		if (arguments.getPackages().size() == 0){
			LOGUE("Specify at least one package!");
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

		auto available = db.getPackage(arguments.getPackages().at(0));
		if (available == nullptr){
			LOGUE("Could not find package in database!");
			return -1;
		}
		LOGU(available->toString());
	}

	{
		std::string out = "Packages to process:";
		for (std::string i : arguments.getPackages())
			out += " " + i;
		LOGU(out);
	}
}
