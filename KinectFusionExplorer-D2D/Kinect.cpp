#include "ICamera.h";
#include "KinectFusionParams.h";
#include "KinectfusionHelper.h";
#include "KinectFusionProcessor.h";
#include "KinectFusionProcessorFrame.h";

#include <iostream>

class Kinect : public ICamera {
private:
	int someParams1;
	int someParams2;

public:
	Kinect(){}

	bool startCamera() {
		std::cout << "starting fake camera ... " << std::endl;
		return true;
	}
};