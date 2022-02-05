
#include <iostream>
#include <string>

#include <fstream>

#include "log.h"
#include "leafdb.h"
#include "arguments.h"

#include "downloader.h"

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

		dl.download("https://raw.githubusercontent.com/AcaciaLinux/leaf_packages/main/packages.list", outFile);

		outFile.close();
		return 0;
	}

	{
		std::string out = "Packages to process:";
		for (std::string i : arguments.getPackages())
			out += " " + i;
		LOGU(out);
	}
}
