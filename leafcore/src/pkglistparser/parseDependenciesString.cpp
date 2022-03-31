/**
 * @file		pkglistparser/parseDependenciesString.cpp
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		The implementation of PackageListParser::parseDependenciesString()
 * @copyright 	Copyright (c) 2022
 */

#include "log.h"
#include "pkglistparser.h"

std::deque<std::string> PackageListParser::parseDependenciesString(std::string deps){
	FUN();

	std::string buf = "";
	std::deque<std::string> list;

	for (char c : deps){
		switch (c){
			case '[':
				buf = "";
				break;

			case ']':
				list.push_back(buf);
				buf = "";
				break;

			default:
				buf += c;
				break;
		}
	}

	return list;
}
