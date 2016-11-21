#pragma once

#include <iostream>

class PLYStorage : public IStorage {
private:

public:
	int save() {
		std::cout << "Saving some file" << std::endl;
		return EXIT_SUCCESS;
	}

	int load() {
		std::cout << "Reading some file !" << std::endl;
		return EXIT_SUCCESS;
	}
};