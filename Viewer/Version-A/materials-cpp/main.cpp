//#include <vtkAutoInit.h>
//VTK_MODULE_INIT(vtkRenderingOpenGL2)

//#include <QApplication>
//#include "window.h"
//#include <QVTKWidget.h>

//#include <boost/make_shared.hpp>
//#include <pcl/point_types.h>
//#include <pcl/point_cloud.h>
//#include <pcl/point_representation.h>

#include <pcl/io/ply_io.h>

//#include <pcl/filters/voxel_grid.h>
//#include <pcl/filters/filter.h>

//#include <pcl/features/normal_3d.h>

//#include <pcl/registration/icp.h>
//#include <pcl/registration/icp_nl.h>
//#include <pcl/registration/transforms.h>

#include <pcl/visualization/pcl_visualizer.h>

//using pcl::visualization::PointCloudColorHandlerGenericField;
//using pcl::visualization::PointCloudColorHandlerCustom;

void ShowPLYColor(char* file_path) {
	pcl::visualization::PCLVisualizer::Ptr pv(new pcl::visualization::PCLVisualizer);
	pv->setSize(700, 700);
	
	int v(20);
	pv->setShowFPS(true);
	pv->createViewPort(0.0, 0.0, 1.0, 1.0, v);

	pcl::PointCloud<pcl::PointXYZRGB>::Ptr src(new pcl::PointCloud<pcl::PointXYZRGB>);
	pcl::io::loadPLYFile<pcl::PointXYZRGB>(file_path, *src);
	// color
	pcl::visualization::PointCloudColorHandlerRGBField<pcl::PointXYZRGB> src_rgb(src);
	// point cloud
	pv->addPointCloud<pcl::PointXYZRGB>(src, src_rgb, "v1_source", v);
	// draw cloud
	pv->spin();
}


int main(int argc, char** argv)
{
	char * pFile = (char *)malloc(64);
	strcpy(pFile, "C:/Users/user/Desktop/PFE/objects/testColor/plan1.ply");
	ShowPLYColor(pFile);
	free(pFile);

	/*QApplication app(argc, argv);
	
	Pclwindow w;
	w.show();

	app.exec();*/



	return EXIT_SUCCESS;
}