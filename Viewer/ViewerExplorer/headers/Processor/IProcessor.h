#pragma once

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



/*
Processor interface to access the core from the UI.
*/

class IProcessor {
private:

public:

	virtual Status merge(const string& from, const string& to, const string& into) = 0;

	virtual Status computeMetrics(float x1, float y1, float z1, float x2, float y2, float z2) = 0;

	//virtual Status reduceSize(string pathToLoad, string pathToSave) = 0;

	//virtual void flatten() = 0;
	//virtual void flattenMesh() = 0;

};