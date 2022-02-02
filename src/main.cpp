
#include <iostream>
#include <string>

#include <log.h>

#include "leafdb.h"
#include "options.h"

bool parse_options(int argc, char** argv);

run_options options;

Log::Log* hlog;

int main(int argc, char** argv){
	hlog = new Log::Log(Log::U);
	hlog->setFeature(Log::FEATURE_PRINTFUNNAMES, false);
	FUN();

	if (!parse_options(argc, argv)){
		return -1;
	}

	LOGU("Welcome to leaf package manager!");

	{
		std::string out = "Packages to process:";
		for (std::string i : options.packages)
			out += " " + i;
		LOGU(out);
	}

	LeafDB db;
	db.newPackage("MyPackage")->addProvidedFile("MyNewFile");
	db.newPackage("SecondPackage")->addProvidedFile("MyNewFile");

	auto providers = db.findFileProviders("MyNewFile");
	LOGU("Providers for file:");
	for (auto i : providers){
		LOGU("-> " + i->getName());
	}
}
