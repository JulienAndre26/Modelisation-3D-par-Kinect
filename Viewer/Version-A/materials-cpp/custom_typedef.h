#pragma once
#include <pcl/point_types.h>
#include <pcl/point_cloud.h>

//convenient typedefs
typedef pcl::PointXYZ PointT;
typedef pcl::PointCloud<PointT> PointCloud;
typedef pcl::PointNormal PointNormalT;
typedef pcl::PointCloud<PointNormalT> PointCloudNormals;
typedef pcl::FPFHSignature33 FPFHSignature33T;
typedef pcl::PointCloud<FPFHSignature33T> PointCloudFPFH;
typedef pcl::PointIndices IndicesT;