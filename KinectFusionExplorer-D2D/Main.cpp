#include <cstdio>
#include <stdio.h>
#include <iostream>
#include <string>

#include "ICamera.h"

using namespace std;

int main() {
	// welcome message and selection
	cout << "Welcome on Modelisation 3D par kinect project !" << endl;
	cout << "Please choose the camera : " << endl;
	cout << "1 - Kinect " << endl;
	cout << "0 - quit" << endl;

	// retrieve user choice 
	string value;
	cin >> value;

	// if asked for 1 start the fake camera
	if (strcmp(value.c_str(), "1") == 0) {
		ICamera * camera = ICamera::factory(1);
		camera->start();
	}

	// return success
	return EXIT_SUCCESS;
}