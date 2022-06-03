#include "log.h"
#include "leafconfig.h"
#include "arguments.h"

#include "args.hxx"

static args::ArgumentParser parser("leaf package manager", "Leaf package manager is the package manager for acacia linux");

bool Arguments::parse(int argc, char** argv){
	FUN();

	args::HelpFlag f_help(parser, "help", "Display this help menu", {'h', "help"});
	args::Flag f_verbose(parser, "verbose", "Display verbose output", {'v'});
	args::Flag f_redownload(parser, "redownload", "Redownload the specified package even if it is in the cache", {"redownload"});
	args::Flag f_redownloadAll(parser, "redownloadAll", "Redownload all the package and dependencies even if they are in the cache", {"redownloadAll"});
	args::Flag f_forceOverwrite(parser, "force overwrite", "Force leaf to ignore file conflicts and write anyway", {"forceOverwrite"});
	args::Flag f_noPreinstall(parser, "skip preinstall", "Do not execute the preinstall script", {"noPreinstall"});
	args::Flag f_noPostinstall(parser, "skip postnstall", "Do not execute the postinstall script", {"noPostinstall"});
	args::ValueFlag<int> f_verbosity(parser, "verbosity", "The verbosity level to use (0, 1, 2, 3)", {"verbosity", 'V'});
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

	if (f_verbosity){
		if (!switchVerbosity(args::get(f_verbosity)))
			return false;
	}

	if (args::get(f_noPreinstall)){
		LOGUW("WARNING: Disabled preinstall scripts, installed packages may not work!");
		_config.runPreinstall = false;
	}

	if (args::get(f_noPostinstall)){
		LOGUW("WARNING: Disabled postinstall scripts, installed packages may not work!");
		_config.runPostinstall = false;
	}

	if (f_rootPath){
		LOGUE("Option --rootPath is now --root");
		LOGUE("Use --root!");
		return false;
	}

	if (f_root){
		if (!expandRootDir(args::get(f_root)))
			return false;
	}

	if (args::get(f_forceOverwrite)){
		LOGUW("WARNING: You use forceOverwrite, leaf will not check for file conflicts!");
		_config.forceOverwrite = true;
	}

	if (args::get(f_verbose)){
		if (!switchVerbosity(1))
			return false;
	}

	if (args::get(f_redownloadAll)){
		_config.redownload = CONFIG_REDOWNLOAD_ALL;
	} else if (args::get(f_redownload)){
		_config.redownload = CONFIG_REDOWNLOAD_SPECIFIED;
	}

	if (!this->setAction(args::get(a_action))){
		std::cout << parser << std::endl;
		return false;
	}

	LOGD("Action to be performed: \"" + args::get(a_action) + "\"");

	LOGD("Packages to operate on:");
	auto packages_res = args::get(a_packages);
	for (auto i : packages_res){
		LOGD(" -> " + i);
		_config.packages.push_back(i);
	}

	return true;
}


