#include "mainwindow.h"
#include <QApplication>
#include "IOPLY.h"
#include "custom_typedef.h"
#include "PCLCore.h"
#include <pcl/ros/conversions.h>
#include <pcl/visualization/pcl_visualizer.h>



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();

	//std::cout << "defining point cloud" << std::endl;
	//pcl::PolygonMesh::Ptr mesh(new PolygonMesh);	
	//PointCloud::Ptr cloud(new PointCloud());
	//IOPLY::load("t1.ply", mesh);


	//pcl::fromROSMsg(mesh->cloud, *cloud);
	//std::cout << "file opened" << std::endl;
	//PCLCore * core = new PCLCore();
	//std::cout << "calling method from core" << std::endl;
	//core->computeBoundingBoxSize(cloud);
	//std::string var;
	//std::cin >> var;
}
