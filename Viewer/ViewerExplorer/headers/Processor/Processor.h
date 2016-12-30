#pragma once

#include "./core/ICore.h"
#include "IProcessor.h"

class Processor : public IProcessor {
private :
	int someField;
	ICore * core;

public:
	// inherited methods ?
	Status merge(string path);
	Status computeMetrics();
	static Status computeMetrics(float x1, float y1, float z1, float x2, float y2, float z2);

};