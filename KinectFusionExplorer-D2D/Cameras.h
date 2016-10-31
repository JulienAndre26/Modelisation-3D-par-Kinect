#pragma once

#include <iostream>

#include "ICamera.h";

// Only import Kinect Fusion explorer 
// as it already includes all other .h 
#include "KinectFusionExplorer.h";


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
	bool start() {
		std::cout << "Asking or Kinect Fustion Processor ... " << std::endl;
		KinectFusionProcessor * kfp = new KinectFusionProcessor();
		kfp->StartProcessing();
		std::cout << "Starting Kinect Processing ... " << std::endl;
		return true;
	}
};

class XTION : public ICamera {
private:
	int someStuff;

public:
	// Constructor
	XTION() {}

	// Destructor
	~XTION() {}

	// inherited method, start the camera
	bool start() {
		std::cout << "Starting fake XTION ... " << std::endl;
		return true;
	}
};