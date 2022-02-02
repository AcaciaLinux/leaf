
#include <iostream>
#include <string>

#include <log.h>

#include "leafdb.h"

Log::Log* hlog;

int main(){
	hlog = new Log::Log(Log::U);
	hlog->setFeature(Log::FEATURE_PRINTFUNNAMES, false);
	
	FUN();

	LOGU("Welcome to leaf package manager!");

	LeafDB db;
	db.newPackage("MyPackage")->addProvidedFile("MyNewFile");
	db.newPackage("SecondPackage")->addProvidedFile("MyNewFile");

	auto providers = db.findFileProviders("MyNewFile");
	LOGU("Providers for file:");
	for (auto i : providers){
		LOGU("-> " + i->getName());
	}
}