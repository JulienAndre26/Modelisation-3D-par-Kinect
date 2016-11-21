/*#include <cstdio>
#include <stdio.h>
#include <iostream>
#include <string>

#include "ICamera.h"
#include "IStorage.h"


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
		IStorage * ply = IStorage::factory(1);
		ply->save();
		ply->load();
	}

	cin >> value;

	// return success
	return EXIT_SUCCESS;
}

*/



#pragma warning( disable : 4996 )
#include <cstdio>
#include <stdio.h>
#include <iostream>
#include <string>

#include <pcl/io/ply_io.h>
#include <pcl/keypoints/uniform_sampling.h>
#include <pcl/visualization/pcl_visualizer.h>

using namespace pcl;
using namespace std;

#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl/visualization/cloud_viewer.h>
#include <pcl/visualization/pcl_visualizer.h>
#include <pcl/io/ply_io.h>

#include <pcl/compression/octree_pointcloud_compression.h>

#include <stdio.h>
#include <sstream>
#include <stdlib.h>

#ifdef WIN32
# define sleep(x) Sleep((x)*1000)
#endif

class SimpleOpenNIViewer
{
public:
	SimpleOpenNIViewer() :
		viewer(new pcl::visualization::PCLVisualizer("3D Viewer"))
	{
		viewer->initCameraParameters();
	}

	void
		cloud_cb_(const pcl::PointCloud<pcl::PointXYZRGB>::ConstPtr &cloud)
	{
		if (!viewer->wasStopped())
		{
			// stringstream to store compressed point cloud
			std::stringstream compressedData;
			// output pointcloud
			pcl::PointCloud<pcl::PointXYZRGB>::Ptr cloudOut(new pcl::PointCloud<pcl::PointXYZRGB>());

			// compress point cloud
			PointCloudEncoder->encodePointCloud(cloud, compressedData);

			// decompress point cloud
			//PointCloudDecoder->decodePointCloud(compressedData, cloudOut);

			// show decompressed point cloud
			int v1(0);
			viewer->createViewPort(0.0, 0.0, 1.0, 1.0, v1);
			viewer->setBackgroundColor(0.2, 0.2, 0.2, v1);
			pcl::visualization::PointCloudColorHandlerRGBField<pcl::PointXYZRGB> rgb(cloud);
			viewer->addPointCloud<pcl::PointXYZRGB>(cloud, rgb, "Before Cloud", v1);

			/*int v2(0);
			viewer->createViewPort(0.0, 0.0, 1.0, 1.0, v2);
			viewer->setBackgroundColor(0.2, 0.2, 0.2, v2);
			pcl::visualization::PointCloudColorHandlerRGBField<pcl::PointXYZRGB> single_color(cloudOut);
			viewer->addPointCloud<pcl::PointXYZRGB>(cloudOut, single_color, "AfterCloud", v2);*/
		}
	}

	void
		run() {
		// for a full list of profiles see: /io/include/pcl/compression/compression_profiles.h
		pcl::io::compression_Profiles_e compressionProfile = pcl::io::MED_RES_ONLINE_COMPRESSION_WITH_COLOR;

		// instantiate point cloud compression for encoding and decoding
		PointCloudEncoder = new pcl::io::OctreePointCloudCompression<pcl::PointXYZRGB>(compressionProfile, true);
		//PointCloudDecoder = new pcl::io::OctreePointCloudCompression<pcl::PointXYZRGB>();

		pcl::PointCloud<pcl::PointXYZRGB>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZRGB>);
		
		// /!\ : change file to open here : 
		string path = "C:\\Users\\user\\VisualStudioProjects\\Modelisation-3D-par-Kinect\\models\\statue.ply";
		pcl::io::loadPLYFile<pcl::PointXYZRGB>(path.c_str(), *cloud);

		cloud_cb_(cloud);

		while (!viewer->wasStopped())
			viewer->spinOnce(100);

		// delete point cloud compression instances
		delete (PointCloudEncoder);
		//delete (PointCloudDecoder);
	}

	boost::shared_ptr<pcl::visualization::PCLVisualizer> viewer;

	pcl::io::OctreePointCloudCompression<pcl::PointXYZRGB>* PointCloudEncoder;
	//pcl::io::OctreePointCloudCompression<pcl::PointXYZRGB>* PointCloudDecoder;

};

int
main(int argc, char **argv)
{
	SimpleOpenNIViewer v;

	v.run();

	return EXIT_SUCCESS;
}

