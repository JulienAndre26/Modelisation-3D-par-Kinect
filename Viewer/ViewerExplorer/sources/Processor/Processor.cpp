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
#include "PCLCore.h"


//#include "metric_visualizer.h"

Status Processor::merge(const string& from, const string& to, const string& into) {
	Status s;
	PCLCore::merge(from, to, into);
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

//void Processor::flatten(pcl::PointCloud<pcl::PointXYZRGB>::Ptr src, pcl::PointCloud<pcl::PointXYZRGB>::Ptr src_projected, bool isLateral)
//{
//	// Create a set of planar coefficients with X=0,Y|Z=1|0
//	pcl::ModelCoefficients::Ptr coefficients(new pcl::ModelCoefficients());
//	coefficients->values.resize(4);
//
//	if (!isLateral)
//	{
//		// LATERAL (from frontside)
//		coefficients->values[0] = coefficients->values[1] = 0;	// X,Y = 0
//		coefficients->values[2] = 1.0;							// Z = 1
//	}
//	else
//	{
//		// PLAN (from upside)
//		coefficients->values[0] = coefficients->values[2] = 0;	// X,Z = 0
//		coefficients->values[1] = 1.0;							// Y = 1
//	}
//	coefficients->values[3] = 0;
//
//	// Create the filtering object
//	pcl::ProjectInliers<pcl::PointXYZRGB> proj;
//	proj.setModelType(pcl::SACMODEL_PLANE);
//	proj.setInputCloud(src);
//	proj.setModelCoefficients(coefficients);
//	proj.filter(*src_projected);
//}

//void Processor::flattenMesh(PolygonMesh::Ptr src, PolygonMesh::Ptr src_projected, bool isLateral)
//{
//	// Create a set of planar coefficients with X=0,Y|Z=1|0
//	pcl::ModelCoefficients::Ptr coefficients(new pcl::ModelCoefficients());
//	coefficients->values.resize(4);
//
//	if (!isLateral)
//	{
//		// LATERAL (from frontside)
//		coefficients->values[0] = coefficients->values[1] = 0;	// X,Y = 0
//		coefficients->values[2] = 1.0;							// Z = 1
//	}
//	else
//	{
//		// PLAN (from upside)
//		coefficients->values[0] = coefficients->values[2] = 0;	// X,Z = 0
//		coefficients->values[1] = 1.0;							// Y = 1
//	}
//	coefficients->values[3] = 0;
//
//	pcl::PCLPointCloud2 mesh_cloud2 = src->cloud;
//	pcl::PointCloud<pcl::PointXYZRGB>::Ptr mesh_cloud(new pcl::PointCloud<pcl::PointXYZRGB>);
//	
//	pcl::PointCloud<pcl::PointXYZRGB>::Ptr mesh_cloud_projected(new pcl::PointCloud<pcl::PointXYZRGB>);
//
//	pcl::fromROSMsg(mesh_cloud2, *mesh_cloud);
//	
//	// Create the filtering object
//	pcl::ProjectInliers<pcl::PointXYZRGB> proj;
//	proj.setModelType(pcl::SACMODEL_PLANE);
//	proj.setInputCloud(mesh_cloud);
//	proj.setModelCoefficients(coefficients);
//	proj.filter(*mesh_cloud_projected);
//
//	pcl::PCLPointCloud2::Ptr mesh_cloud2_projected(new pcl::PCLPointCloud2);
//	pcl::toROSMsg(*mesh_cloud_projected, *mesh_cloud2_projected);
//	src_projected->cloud = *mesh_cloud2_projected;
//}

std::map<std::string, double>* Processor::computeBoundingBox(pcl::PolygonMesh::Ptr mesh) {
	PointCloud::Ptr cloud(new PointCloud());
	pcl::fromROSMsg(mesh->cloud, *cloud);
	return Processor::computeBoundingBox(cloud);
}

std::map<std::string, double>* Processor::computeBoundingBox(pcl::PointCloud<pcl::PointXYZ>::Ptr cloud) {
	std::map<std::string, double> * result = new std::map<std::string, double>();

	std::vector<double> resultArray = PCLCore::computeBoundingBoxSize(cloud);
	if (resultArray.size() < 3) {
		(*result)["x"] = 0.0;
		(*result)["y"] = 0.0;
		(*result)["z"] = 0.0;
	} else {
		(*result)["x"] = resultArray[0];
		(*result)["y"] = resultArray[1];
		(*result)["z"] = resultArray[2];
	}
	
	return result;
}

std::map<std::string, double>* Processor::computeBoundingBox(pcl::PointCloud<pcl::PointXYZRGB>::Ptr cloud) {
	pcl::PointCloud<pcl::PointXYZ>::Ptr noColor(new PointCloud);
	pcl::copyPointCloud(*cloud, *noColor);
	return Processor::computeBoundingBox(noColor);
}