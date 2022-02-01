
#include <iostream>
#include <string>

#include <log.h>

#include "file.h"
#include "package.h"

Log::Log* hlog;

int main(){
	hlog = new Log::Log(Log::F);
	hlog->setFeature(Log::FEATURE_PRINTFUNNAMES, true);
	
	FUN();

	LOGU("Welcome to leaf package manager!");

	File file("MyNewFile", "////hello/akdh/kdafjn", "mypackage");
	Package package("mypackage");
}
