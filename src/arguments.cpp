#include "log.h"
#include "leafconfig.h"
#include "arguments.h"

#include "args.hxx"

static args::ArgumentParser parser("leaf package manager", "Leaf package manager is the package manager for acacia linux");

bool Arguments::parse(int argc, char** argv){
	FUN();

	args::HelpFlag f_help(parser, "help", "Display this help menu", {'h', "help"});
	args::Flag f_verbose(parser, "verbose", "Display verbose output", {'v'});
	args::Flag f_sVerbose(parser, "super verbose", "Display eben more verbose output", {'V'});
	args::Flag f_redownload(parser, "redownload", "Redownload the specified package even if it is in the cache", {"redownload"});
	args::ValueFlag<std::string> f_rootPath(parser, "rootpath", "The root path leaf deploys its packages to", {"rootPath"});
	args::ValueFlag<std::string> f_root(parser, "root", "The root leaf should work on", {"root"});
	args::Positional<std::string> a_action(parser, "Action", "The action that should be performed by leaf");
	args::PositionalList<std::string> a_packages(parser, "packages", "The packages to operate on");

	try
	{
		parser.ParseCLI(argc, argv);
	}
	catch (const args::Completion& e)
	{
		LOGE(e.what());
		return false;
	}
	catch (const args::Help&)
	{
		std::cout << parser;
		return false;
	}

	catch (const args::ParseError& e)
	{
		LOGE(std::string("Failed to parse options: ") + std::string(e.what()));
		std::cerr << parser;
		return false;
	}

	if (f_rootPath){
		LOGUE("Option --rootPath is now --root");
		LOGUE("Use --root!");
		return false;
	}

	if (f_root){
		lConfig.rootDir = args::get(f_root);
		LOGD("Using root path " + lConfig.rootDir);
	}

	if (args::get(f_verbose)){
		lConfig.verbosity = CONFIG_V_VERBOSE;
		hlog->setLevel(Log::I);
	}

	if (args::get(f_sVerbose)){
		lConfig.verbosity = CONFIG_V_SUPERVERBOSE;
		hlog->setLevel(Log::D);
	}

	if (args::get(f_redownload)){
		lConfig.redownload = CONFIG_REDOWNLOAD_SPECIFIED;
	}

	if (!this->setAction(args::get(a_action)))
		return false;

	LOGD("Action to be performed: \"" + args::get(a_action) + "\"");

	LOGD("Packages to operate on:");
	auto packages_res = args::get(a_packages);
	for (auto i : packages_res){
		LOGD(" -> " + i);
		lConfig.packages.push_back(i);
	}

	return true;
}

bool Arguments::setAction(std::string a){
	FUN();

	if (a == "update")
		lConfig.action = ACTION_UPDATE;
	else if (a == "install")
		lConfig.action = ACTION_INSTALL;

	else{
		std::cout << parser;
		return false;
	}

	return true;
}
