#include "Processor.h"

#include <iostream>
//#include <boost/make_shared.hpp>
//#include <boost/thread/thread.hpp>
//#include <pcl/common/common_headers.h>
//#include <pcl/features/normal_3d.h>
//#include <pcl/features/fpfh.h>
//#include <pcl/features/boundary.h>
//#include <pcl/filters/voxel_grid.h>
//#include <pcl/filters/filter.h>
//#include <pcl/io/ply_io.h>
//#include <pcl/keypoints/iss_3d.h>
//#include <pcl/registration/correspondence_estimation.h>
//#include <pcl/registration/correspondence_rejection_sample_consensus.h>
//#include <pcl/registration/icp.h>
//#include <pcl/registration/icp_nl.h>
//#include <pcl/registration/transforms.h>
//#include <pcl/registration/transformation_estimation_svd.h>
//#include <pcl/visualization/pcl_visualizer.h>
//#include <pcl/filters/bilateral.h>
//#include <pcl/filters/impl/bilateral.hpp>
//#include <pcl/filters/fast_bilateral.h>
//#include <pcl/filters/statistical_outlier_removal.h>
//#include <pcl/surface/mls.h>
//#include <pcl/point_representation.h>

#include "custom_typedef.h"
#include "IOPLY.h"


//#include "metric_visualizer.h"

Status Processor::merge(string path) {
	Status s;
	// todo : for all ply in the given directory
	// call core->merge(file1, file2);
	return s;
}

Status Processor::computeMetrics() {
	Status s;
	return s;
}

Status Processor::computeMetrics(float x1, float y1, float z1, float x2, float y2, float z2) {
	Status s;
	s.returnedDouble = sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2) + pow(z1 - z2, 2));
	
	cout << endl;
	cout << "P1: (" << x1 << "; " << y1 << "; " << z1 << ")" << endl;
	cout << "P2: (" << x2 << "; " << y2 << "; " << z2 << ")" << endl;
	cout << "DISTANCE: " << s.returnedDouble << endl;
	cout << endl;

	return s;
}

