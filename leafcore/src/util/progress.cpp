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

#ifdef LEAF_NCURSES
	#include <curses.h>
#endif

static bool progress_printed = false;
static bool printed_warning_fallback = false;
static uint64_t screen_width = 0;
static uint64_t last_percentage = 0;

void LeafUtil::Progress::init(){
	FUN();

	#ifdef LEAF_NCURSES
		int col, row;
		char* cTerm = getenv("TERM");

		std::string term;
		if (cTerm != NULL){
			term = std::string(cTerm);
		}

		if (term.length() == 0){
			if (!printed_warning_fallback){
				LOGUE("Terminal type is not set (TERM=), falling back!");
				printed_warning_fallback = true;
			}
			screen_width = 0;
		} else {
			if (initscr() == nullptr){
				refresh();
				endwin();
				if (!printed_warning_fallback){
					LOGUE("Could not determine terminal width, falling back!");
					printed_warning_fallback = true;
				}
				screen_width = 0;
			} else {
				getmaxyx(stdscr, row, col);
				refresh();
				endwin();
				screen_width = col;
			}
		}

		LOGI("[LeafUtil][Progress][init] Screen width = " + std::to_string(screen_width));
	#endif

	last_percentage = 0;
}

void LeafUtil::Progress::print(std::string prefix, uint64_t total, uint64_t progress, uint64_t steps, std::string preProgress){
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

	if (percentage == last_percentage)
		return;

	uint64_t spaces_prefix_progress = 0;
	if (screen_width != 0){
		spaces_prefix_progress = screen_width - steps - prefix.size() - 11 - preProgress.length();
	}

	progress_printed = true;

	std::cout << "\033[2K  " << prefix << std::string(spaces_prefix_progress, ' ') << preProgress << " [" << std::string(steps_complete, '#') << std::string(steps_missing, ' ') << "] " << percentage << "%\r";
	std::cout.flush();
}

void LeafUtil::Progress::end(){
	FUN();

	if (progress_printed){
		std::cout << std::endl;
		progress_printed = false;
	}
}
