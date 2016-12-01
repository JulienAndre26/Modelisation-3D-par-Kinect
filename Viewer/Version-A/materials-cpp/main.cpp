#include "window.h"
#include <QApplication>
#include <QMainWindow>

#include <iostream>
#include <QMainWindow>

#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl/visualization/pcl_visualizer.h>

#include <QVTKWidget.h>
#include <vtkRenderWindow.h>

typedef pcl::PointXYZRGBA PointT;
typedef pcl::PointCloud<PointT> PointCloudT;

boost::shared_ptr<pcl::visualization::PCLVisualizer> viewer;
PointCloudT::Ptr cloud;

unsigned int red;
unsigned int green;
unsigned int blue;

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	QVTKWidget qvtkWidget;
	
	// Setup the cloud pointer
	cloud.reset(new PointCloudT);
	// The number of points in the cloud
	cloud->points.resize(200);

	// The default color
	red = 128;
	green = 128;
	blue = 128;

	// Fill the cloud with some points
	for (size_t i = 0; i < cloud->points.size(); ++i)
	{
		cloud->points[i].x = 1024 * rand() / (RAND_MAX + 1.0f);
		cloud->points[i].y = 1024 * rand() / (RAND_MAX + 1.0f);
		cloud->points[i].z = 1024 * rand() / (RAND_MAX + 1.0f);

		cloud->points[i].r = red;
		cloud->points[i].g = green;
		cloud->points[i].b = blue;
	}

	// Set up the QVTK window
	viewer.reset(new pcl::visualization::PCLVisualizer("viewer", false));
	qvtkWidget.SetRenderWindow(viewer->getRenderWindow());
	viewer->setupInteractor(qvtkWidget.GetInteractor(), qvtkWidget.GetRenderWindow());
	qvtkWidget.update();

	viewer->addPointCloud(cloud, "cloud");
	viewer->resetCamera();
	qvtkWidget.update();

	return a.exec();
}