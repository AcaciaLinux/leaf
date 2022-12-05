/**
 * @file		
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		The implementation of 
 * @copyright	Copyright (c) 2022
 */
#include "log.h"
#include "leafdebug.h"
#include "leafconfig.h"

#include "leafcore.h"

void Leafcore::a_remove(std::deque<std::string> packages){
	FUN();
	LEAF_DEBUG_EX("Leafcore::a_remove()");

	checkDirectories();

	parseInstalled();

	std::deque<Package*> remove_packages;
	{//Resolve the packages names
		for (std::string packageName : packages){
			//This throws an error if the package was not found
			Package* package = _installedDB->getPackage(packageName, true);

			remove_packages.push_back(package);
		}
	}

	//Check if there are packages that depend on the removed package(s)	
	{
		std::deque<Package*> all_packages;
		all_packages.insert(all_packages.end(), remove_packages.begin(), remove_packages.end());

		//Resolve the dependers and remove the packages that are specified, remaining the additional packages
		_installedDB->resolveDependers(all_packages);
		all_packages.erase(all_packages.begin(), all_packages.begin() + remove_packages.size());

		if (all_packages.size() > 0){

			//Inform the user about depending packages
			std::string outString = "The following packages " + std::string(_config.force ? "would " : " ") + "depend on the removed package(s):";
			for (Package* p : all_packages){
				outString += "\n\t-> " + p->getFullName() + "";
			}

			if (!_config.force){
				LOGU(outString);

				//Ask if the depending packages should be removed too
				bool removeDependers = askUserOK("Do you want to remove them too?", false);

				//If so, add the depending packages to the remove_packages list
				if (removeDependers)
					remove_packages.insert(remove_packages.end(), all_packages.begin(), all_packages.end());
				else
					throw new LeafError(Error::USER_DISAGREE, "Removing depending packages");
			} else {
				LOGUW("WARNING: Using 'force', leaf does not check for depending packages (you may remove dependencies for other packages)!");
				LOGU(outString);
			}
		}
	}

	//Inform the user
	{
		std::string outString = "Packages to remove:";
		for (Package* p : remove_packages){
			outString += " " + p->getFullName();
		}
		LOGU(outString);
	}

	{//Ask the user for permission
		if (!askUserOK("Do you want to continue?", true)){
			std::string msg = "Removing packages:";
			for (Package* pkg : remove_packages)
				msg += " " + pkg->getFullName();
			throw new LeafError(Error::USER_DISAGREE, msg);
		}
	}

	{//Execute pre-action hooks
		LOGU("Running pre remove hooks...");
		for (Hook& hook : _hooks){
			LOGD("Running pre remove hook...");
			hook.execPre(_config);
		}
	}

	for (Package* p : remove_packages){
		LOGU("Removing package " + p->getFullName() + "...");

		p->removeFromRoot();
	}

	{//Execute post-action hooks
		LOGU("Running post remove hooks...");
		for (Hook& hook : _hooks){
			LOGD("Running post remove hook...");
			hook.execPost(_config);
		}
	}
}
