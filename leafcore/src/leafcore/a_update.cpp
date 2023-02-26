/**
 * @file		leafcore/a_update.cpp
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		The implementation of Leafcore::a_update()
 * @copyright	Copyright (c) 2022
 */
#include "log.h"
#include "leafdebug.h"
#include "leafconfig.h"

#include "leafcore.h"
#include "downloader.h"
#include "branchmaster.h"

#include <fstream>
#include <nlohmann/json.hpp>

void Leafcore::a_update(){
	FUN();
	LEAF_DEBUG_EX("Leafcore::a_update()");

	checkDirectories();

	std::string pkgListFilePath = _config.pkgListPath();
	LOGI("Fetching package list from " + _config.pkgListURL + " to " + pkgListFilePath);

	//The temporary storage for the packagelist data
	std::stringstream ss_pkglist;

	//Create the downloader instance and download the file
	Downloader dl(_config.pkgListURL, ss_pkglist, _config.noProgress);
	dl.init();
	size_t dRes = dl.download();

	//Handle the HTTP response code
	if (dRes >= 400){
		throw new LeafError(
			Error::DL_BAD_RESPONSE,
			"When fetching packagelist from " + _config.pkgListURL + ": " +
			std::to_string(dRes));
	}

	BranchResponse response;

	//Try parsing the JSON data
	try {
		nlohmann::json json_res = nlohmann::json::parse(ss_pkglist, nullptr, true);

		//Transfer the data to the struct
		response.response_code = json_res.at("response_code");
		response.status = json_res.at("status");
		response.payload = json_res.at("payload").dump();

	} catch (nlohmann::json::parse_error& e){
		LOGUW("Failed to parse package list response from " + _config.pkgListURL + ", is the URL valid?");
		throw new LeafError(Error::JSON_PARSE, e.what());
	} catch (nlohmann::json::out_of_range& e){
		LOGUW("Failed to parse package list response from " + _config.pkgListURL + ", is the URL valid?");
		throw new LeafError(Error::JSON_OUT_OF_RANGE, e.what());
	}

	//Check the response code
	if (response.response_code != 200){
		throw new LeafError(
			Error::BRANCHMASTER_ERROR,
			"When fetching packagelist from " + _config.pkgListURL + ": " +
			response.payload);
	}

	//Create the output file stream, check it and write the payload
	{
		std::ofstream outFile;
		outFile.open(pkgListFilePath, std::ios::trunc);

		if (!outFile.is_open()){
			outFile.close();
			throw new LeafError(Error::OPENFILEW, pkgListFilePath);
		}

		LOGI("Writing out package list file to " + pkgListFilePath);
		outFile << response.payload << std::endl;

		outFile.close();
	}
}
