#pragma once

class ICamera {

public:
	// standard destructor needed with some compilers
	virtual ~ICamera(){}

	// static factory
	static ICamera * factory(int selection);

	// start camera
	virtual bool start() = 0;


};