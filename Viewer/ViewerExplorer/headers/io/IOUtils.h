#pragma once

#include <iostream>
#include <fstream>

class IOUtils {

public:
	static bool fileExists(std::string sPath) {
		std::ifstream f(sPath.c_str());
		bool bExists = f.good();
		f.close();
		return bExists;
	}
};