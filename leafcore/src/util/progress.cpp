/**
 * @file		util/print_progress.cpp
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		The implementation of LeafUtil::print_progress()
 * @copyright	Copyright (c) 2022
 */
#include "log.h"
#include "error.h"
#include "leafdebug.h"

#include "util.h"

#include <iostream>

static bool progress_printed = false;

void LeafUtil::Progress::print(std::string prefix, uint64_t total, uint64_t progress, uint64_t steps){
	FUN();

	if (total == 0)
		return;
	
	if (progress > total)
		return;

	uint64_t progress_per_step = total / steps;
	uint64_t steps_complete = progress / progress_per_step;
    uint64_t steps_missing = steps - steps_complete;
    uint64_t percentage = ((double)progress / (double)total) * 100.0;

	if (steps_complete > steps || steps_missing > steps)
		return;

	progress_printed = true;

	std::cout << "\033[2K " << prefix << " [" << std::string(steps_complete, '#') << "" << std::string(steps_missing, ' ') << "] " << percentage << "%\r";
	std::cout.flush();
}

void LeafUtil::Progress::end(){
	FUN();

	if (progress_printed){
		std::cout << std::endl;
		progress_printed = false;
	}
}
