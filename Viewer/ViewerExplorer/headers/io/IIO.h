#pragma once

#include <iostream>
#include <fstream>

/*
* Abstract class to 
*/
class IIO {

public:
	int load(const char * filename, void* objectToLoad);
	int save(const char * filename, void* objectToSave);

	static bool fileExists(std::string sPath) {
		std::ifstream f(sPath.c_str());
		bool bExists = f.good();
		f.close();
		return bExists;
	}
};