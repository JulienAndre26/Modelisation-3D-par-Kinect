#include "mainwindow.h"
#include <QApplication>
#include "IOPLY.h"
#include "custom_typedef.h"
#include "PCLCore.h"
#include <pcl/ros/conversions.h>
#include <pcl/visualization/pcl_visualizer.h>
//#include "Processor.h"



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();

	//std::cout << "defining point cloud" << std::endl;
	//pcl::PolygonMesh::Ptr mesh(new PolygonMesh);	
	//pcl::PointCloud<pcl::PointXYZRGB>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZRGB>());
	//IOPLY::load("mergedPC.ply", mesh);


	//pcl::fromROSMsg(mesh->cloud, *cloud);
	//std::cout << "file opened" << std::endl;
	//Processor::computeBoundingBox(cloud);
	//std::string var;
	//std::cin >> var;
}
