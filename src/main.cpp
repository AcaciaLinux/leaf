
#include <iostream>
#include <string>

#include <log.h>

#include "leafdb.h"
#include "arguments.h"

Arguments arguments;

Log::Log* hlog;

int main(int argc, char** argv){
	hlog = new Log::Log(Log::U);
	hlog->setFeature(Log::FEATURE_PRINTFUNNAMES, false);
	FUN();

	if (!arguments.parse(argc, argv)){
		return -1;
	}

	{
		std::string out = "Packages to process:";
		for (std::string i : arguments.getPackages())
			out += " " + i;
		LOGU(out);
	}
}
