#pragma once

#ifndef CTYPEDEF
#define CTYPEDEF

#include <pcl/point_types.h>
#include <pcl/point_cloud.h>

#include <pcl/visualization/pcl_visualizer.h>

//convenient typedefs
typedef pcl::PointXYZ						PointT;
typedef pcl::PointXYZRGB					PointColorT;
typedef pcl::PointCloud<PointT>				PointCloud;
typedef pcl::PointCloud<PointColorT>		PointCloudColored;
typedef pcl::PointNormal					PointNormalT;
typedef pcl::PointCloud<PointNormalT>		PointCloudNormals;
typedef pcl::FPFHSignature33				FPFHSignature33T;
typedef pcl::PointCloud<FPFHSignature33T>	PointCloudFPFH;
typedef pcl::PointIndices					IndicesT;
typedef pcl::visualization::PCLVisualizer	Visualizer;

#endif