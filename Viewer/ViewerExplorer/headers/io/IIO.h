#pragma once

#include <iostream>
#include <fstream>

/*
* Abstract class to 
*/
class IIO {

public:
	virtual int load(const char * filename, void* objectToLoad) = 0;
	virtual int save(const char * filename, void* objectToSave) = 0;

	static bool fileExists(std::string sPath) {
		std::ifstream f(sPath.c_str());
		bool bExists = f.good();
		f.close();
		return bExists;
	}
};