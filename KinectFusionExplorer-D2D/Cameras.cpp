#include "Cameras.h"
#include "ICamera.h"

/*
*	Factory method : it allows to create different
* subclasses of the same interface.
*/
ICamera * ICamera::factory(int selection) {
	switch (selection) {
		// if the user asked for a 1, return kinect reference
		case 1:
			return new Kinect;

		default:
			break;
	}
}
