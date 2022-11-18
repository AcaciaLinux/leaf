/**
 * @file		leafcore/askUserOK.cpp
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		The implementation of askUserOK()
 * @copyright	Copyright (c) 2022
 */

#include "log.h"
#include "leafdebug.h"
#include "leafconfig.h"

#include "leafcore.h"

#include <filesystem>
#include <iostream>

bool Leafcore::askUserOK(std::string question, bool defaultOption){
	FUN();
	LEAF_DEBUG("Leafcore::askUserOK()");

	if (_config.noAsk)
		return true;

	LEAF_DEBUG("Leafcore::askUserOK()-noAsk");

	if (defaultOption){
		LOGD("User question: Default action: true");

		std::cout << question << " (Y/n): ";
		std::string answer;
		getline(std::cin, answer);

		LOGD("User question: Answer: \"" + answer + "\"");
		
		if (answer == "y" || answer == "Y" || answer == ""){
			LOGD("User question: User agreed");
			return true;
		} else {
			LOGD("User question: User disagreed");
			return false;
		}
	} 
	else
	{
		std::cout << question << " (y/N): ";
		std::string answer;
		getline(std::cin, answer);
		
		if (answer == "n" || answer == "N" || answer == "")
			return false;
		else 
			return true;
	}
}
