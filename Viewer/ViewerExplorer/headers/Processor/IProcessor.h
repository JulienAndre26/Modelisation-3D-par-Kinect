#pragma once

#ifndef IPROCESSOR
#define IPROCESSOR

#include <string>

using namespace std;

// can be changed if necessary
typedef struct {
	string returnedString;
	int returnedInt;
	double returnedDouble;
	int code;
	char * message;
} Status;

class IProcessor {
private:

public:

	virtual Status merge(string path) = 0;

	virtual Status computeMetrics() = 0;

	//static Status computeMetrics(float x1, float y1, float z1, float x2, float y2, float z2);

	//virtual Status reduceSize(string pathToLoad, string pathToSave) = 0;

};
#endif