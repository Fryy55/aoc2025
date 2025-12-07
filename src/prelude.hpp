#pragma once


#define AOC_PRELUDE \
    log::set12hTimeEnabled(true); \
	ThreadManager::get()->addThread("Main"); \
\
	std::ifstream I("../data/input.txt"); \
	if (!I) { \
		log::error("Failed to open the input file!"); \
		return 1; \
	} else { \
		log::debug("input.txt opened successfully.\n"); \
	}