#include "log.h"
#include "arguments.h"

#include "args.hxx"

bool Arguments::parse(int argc, char** argv){
	FUN();

	args::ArgumentParser parser("leaf package manager", "Leaf package manager is the package manager for acacia linux");
	args::HelpFlag f_help(parser, "help", "Display this help menu", {'h', "help"});
	args::Flag f_verbose(parser, "verbose", "Display verbose output", {'v'});
	args::ValueFlag<std::string> f_rootPath(parser, "rootpath", "The root path leaf deploys its packages to", {"rootPath"});
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
		this->rootPath = args::get(f_rootPath);
		LOGD("Using root path " + this->rootPath);
	}

	this->verbose = args::get(f_verbose);
	if (this->verbose)
		hlog->setLevel(Log::D);

	if (!this->setAction(args::get(a_action)))
		return false;

	LOGD("Action to be performed: \"" + args::get(a_action) + "\"");

	LOGD("Packages to operate on:");
	auto packages_res = args::get(a_packages);
	for (auto i : packages_res){
		LOGD(" -> " + i);
		this->packages.push_back(i);
	}

	return true;
}

bool Arguments::setAction(std::string a){
	FUN();

	if (a == "update")
		this->action = ACTION_UPDATE;
	else if (a == "install")
		this->action = ACTION_INSTALL;
	else if (a == "remove")
		this->action = ACTION_REMOVE;

	else{
		LOGUE("Unsupported action \"" + a + "\"");
		return false;
	}

	return true;
}

bool Arguments::getVerbose(){
	return this->verbose;
}

e_action Arguments::getAction(){
	return this->action;
}

std::string Arguments::getRootPath(){
	return this->rootPath;
}

std::vector<std::string> Arguments::getPackages(){
	return this->packages;
}
