
#include <iostream>
#include <string>

#include <log.h>

#include "leafdb.h"

Log::Log* hlog;

int main(){
	hlog = new Log::Log(Log::F);
	hlog->setFeature(Log::FEATURE_PRINTFUNNAMES, false);
	
	FUN();

	LOGU("Welcome to leaf package manager!");

	Package package("mypackage");
	package.addProvidedFile("/home/myfile");
	package.addProvidedFile("/home/myuser/mycofig");

	LOGU("Files provided by package \"" + package.getName() + "\":");
	auto files = package.getProvidedFiles();
	for(std::string file : files){
		LOGU("-> " + file);
	}

	if (package.checkFileProvided("/home/myfile")){
		LOGU("Package " + package.getName() + " provides file /home/myfile!");
	}
}