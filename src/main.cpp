#include <aurora/aurora.hpp>

#include <fstream>


int main() {
	log::set12hTimeEnabled(true);
	ThreadManager::get()->addThread("Main");

	std::ifstream I("../data/input.txt");
	if (!I) {
		log::error("Failed to open the input file!");
		return 1;
	} else {
		log::debug("input.txt opened successfully.\n");
	}

	
	I.close();

	return 0;
}