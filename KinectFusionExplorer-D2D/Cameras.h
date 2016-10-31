#pragma once

#include <iostream>

#include "ICamera.h";

// Error while including ...
//#include "KinectFusionParams.h";
//#include "KinectfusionHelper.h";
//#include "KinectFusionProcessor.h";
//#include "KinectFusionProcessorFrame.h";


class Kinect : public ICamera {
private:
	int someParams1;
	int someParams2;

public:
	// Constructor
	Kinect() {}

	// Destructor
	~Kinect() {}

	// inherited method, start the camera
	bool startCamera() {
		std::cout << "starting fake camera ... " << std::endl;
		return true;
	}
};