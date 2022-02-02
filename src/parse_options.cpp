#include "log.h"
#include "options.h"
#include "args.hxx"

bool parse_options(int argc, char** argv){
	FUN();
	args::ArgumentParser parser("leaf package manager", "Leaf package manager is the package manager for acacia linux");
    args::HelpFlag help(parser, "help", "Display this help menu", {'h', "help"});
	args::Flag verbose(parser, "verbose", "Display verbose output", {'v'});
	args::PositionalList<std::string> packages(parser, "packages", "The packages to operate on");

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

	options.verbose = args::get(verbose);
	if (options.verbose)
		hlog->setLevel(Log::D);

    LOGD("Packages to operate on:");
	auto packages_res = args::get(packages);
	for (auto i : packages_res){
		LOGD("-> " + i);
		options.packages.push_back(i);
	}

	

	return true;
}
