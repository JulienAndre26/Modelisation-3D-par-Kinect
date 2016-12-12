#pragma once


#include "custom_typedef.h"
#include "Processor.h"

struct callback_args {
	// structure used to pass arguments to the callback function
	PointCloud::Ptr clicked_points_3d;
	Visualizer::Ptr viewerPtr;
	char * distance;
};


void pp_callback(const pcl::visualization::PointPickingEvent& event, void* args)
{
	// retrieve parameter
	struct callback_args* data = (struct callback_args*)args;
	// in case not point found, exit
	if (event.getPointIndex() == -1)
		return;

	// instanciate point
	PointT current_point;

	// retrieve values
	event.getPoint(current_point.x, current_point.y, current_point.z);

	// if two points are already in the list
	if (data->clicked_points_3d->points.size() == 2) {
		// empty the list
		data->clicked_points_3d->points.clear();
		// reset distance value
		sprintf(data->distance, "%f", 0);
	}

	// add point to the list
	data->clicked_points_3d->points.push_back(current_point);

	// if there are two points in the list
	if (data->clicked_points_3d->points.size() == 2) {
		double dist;
		// retrieve values
		PointT point1 = data->clicked_points_3d->points.at(0);
		PointT point2 = data->clicked_points_3d->points.at(1);

		// compute distance
		dist = Processor::computeMetrics(point1.x, point2.x, point1.y, point2.y, point1.z, point2.z).returnedDouble;
		// set value
		sprintf(data->distance, "%f", dist);
		// debug it
		std::cout << "DISTANCE : " << data->distance << std::endl;
	}

	// Draw clicked points in red:
	pcl::visualization::PointCloudColorHandlerCustom<PointT> red(data->clicked_points_3d, 255, 0, 0);
	data->viewerPtr->removePointCloud("clicked_points");
	data->viewerPtr->addPointCloud(data->clicked_points_3d, red, "clicked_points");
	data->viewerPtr->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 10, "clicked_points");
	// debug 
	std::cout << current_point.x << " " << current_point.y << " " << current_point.z << std::endl;
}
class MetricVisualizer : public Visualizer  {
private:
	struct callback_args args;

public:

	MetricVisualizer(char * framename, PointCloud::Ptr k) {
		int v(123);
		//embededViewer = new Visualizer(framename, false);
		PointCloud::Ptr clicked_points_3d(new PointCloud);
		args.clicked_points_3d = clicked_points_3d;
		args.viewerPtr = Visualizer::Ptr(this);
		args.distance = new char[128];
		sprintf(args.distance, "%f", 0);

		this->setWindowName(framename);
		this->setShowFPS(true);
		this->createViewPort(0.0, 0.0, 1.0, 1.0, v);
		this->setPosition(0, 0);
		// no color management yet
		this->addPointCloud(k);
		this->registerPointPickingCallback(pp_callback, (void*) &args);
		this->spin();
	}

	~MetricVisualizer() {};

};