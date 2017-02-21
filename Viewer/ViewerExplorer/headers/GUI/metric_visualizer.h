#pragma once

#include "custom_typedef.h"
#include "Processor.h"
#include "mainwindow.h"

struct callback_args {
	// structure used to pass arguments to the callback function
	PointCloud::Ptr clicked_points_3d;
	Visualizer::Ptr viewerPtr;
	char * distance;
	MainWindow * ui;
};

void _callback(const pcl::visualization::PointPickingEvent& event, void* args)
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

	//cout << "Clicked P: (" << current_point.x << "; " << current_point.y << "; " << current_point.z << ")" << endl;
	
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

		Processor proc;

		// compute distance
		dist = proc.computeMetrics(point1.x, point1.y, point1.z, point2.x, point2.y, point2.z).returnedDouble;
		// set value
		sprintf(data->distance, "%f", dist);
		// debug it
		//std::cout << "DISTANCE : " << data->distance << std::endl;

		data->ui->updateMetrics(METRIC_P2, point2.x, point2.y, point2.z);
		data->ui->updateMetrics(dist);
		
		data->viewerPtr->addLine<PointT>(point1, point2, "line");
	}
	else
	{
		PointT point1 = data->clicked_points_3d->points.at(0);
		data->ui->updateMetrics(METRIC_P1, point1.x, point1.y, point1.z);

		data->viewerPtr->removeShape("line");
	}

	// Draw clicked points in red:
	pcl::visualization::PointCloudColorHandlerCustom<PointT> red(data->clicked_points_3d, 255, 0, 0);
	data->viewerPtr->removePointCloud("clicked_points");
	data->viewerPtr->addPointCloud(data->clicked_points_3d, red, "clicked_points");
	data->viewerPtr->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 10, "clicked_points");

	// debug 
	//std::cout << current_point.x << " " << current_point.y << " " << current_point.z << std::endl;
}

class MetricVisualizer : public Visualizer  {
private:
	struct callback_args args;

public:

	MetricVisualizer(PointCloudColored::Ptr src, bool bColored, MainWindow * mw) 
		: Visualizer("", false) 
	{
		init(mw);

		if (!bColored) // Add custom color if the point cloud is not colored
		{
			pcl::PointCloud<PointT>::Ptr src_custom(new pcl::PointCloud<PointT>);

			for (int i = 0; i < src->size(); i++)
				src_custom->push_back(PointT(src->at(i).x, src->at(i).y, src->at(i).z));

			pcl::visualization::PointCloudColorHandlerCustom<PointT> src_rgb(src_custom, 50, 210, 210);
			this->addPointCloud<PointT>(src_custom, src_rgb, "v1_source", 0);
		} 
		else // Already colored
		{
			pcl::visualization::PointCloudColorHandlerRGBField<pcl::PointXYZRGB> src_rgb(src);
			this->addPointCloud<PointColorT>(src, src_rgb, "v1_source", 0);
		} 
	}

	MetricVisualizer(PolygonMesh::Ptr src, MainWindow * mw)
		: Visualizer() 
	{
		init(mw);	
		this->addPolygonMesh(*src);
	}

	void init(MainWindow * mw) {
		// Data Structure
		PointCloud::Ptr clicked_points_3d(new PointCloud);
		args.clicked_points_3d = clicked_points_3d;
		args.viewerPtr = Visualizer::Ptr(this);
		args.distance = new char[128];
		args.ui = mw;
		sprintf(args.distance, "%f", 0);

		// Event Listener
		//this->createInteractor();
		this->registerPointPickingCallback(_callback, (void*)&args);
	}

	~MetricVisualizer() {};

};