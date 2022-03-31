/**
 * @file		pkglistparser/pkglistparser.cpp
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		The core implementation of PackageListParser
 * @copyright 	Copyright (c) 2022
 */

#include "log.h"
#include "pkglistparser.h"

PackageListParser::~PackageListParser(){
	FUN();

	for (Package* package : _packages)
		delete package;
}
