#include "ProcessorPCL.h"

#include <iostream>
#include <boost/make_shared.hpp>
#include <boost/thread/thread.hpp>
#include <pcl/common/common_headers.h>
#include <pcl/features/normal_3d.h>
#include <pcl/features/fpfh.h>
#include <pcl/filters/voxel_grid.h>
#include <pcl/filters/filter.h>
#include <pcl/io/ply_io.h>
#include <pcl/keypoints/iss_3d.h>
#include <pcl/registration/correspondence_estimation.h>
#include <pcl/registration/correspondence_rejection_sample_consensus.h>
#include <pcl/registration/icp.h>
#include <pcl/registration/icp_nl.h>
#include <pcl/registration/transforms.h>
#include <pcl/registration/transformation_estimation_svd.h>
#include <pcl/visualization/pcl_visualizer.h>
#include <pcl/filters/bilateral.h>
#include <pcl/filters/impl/bilateral.hpp>
#include <pcl/filters/fast_bilateral.h>
#include <pcl/filters/statistical_outlier_removal.h>
#include <pcl/surface/mls.h>
#include <pcl/point_representation.h>

#include "custom_typedef.h"

using pcl::visualization::PointCloudColorHandlerGenericField;
using pcl::visualization::PointCloudColorHandlerCustom;


// COLORED VERSION
void reduce_size(PointCloudColored::Ptr cloud, PointCloudColored::Ptr res) {
	cout << "PointCloud Colored before filtering: " << cloud->width * cloud->height
		<< " data points (" << pcl::getFieldsList(*cloud) << ")." << endl;

	
	// Create the filtering object
	pcl::VoxelGrid<PointColorT> colored_vg;
	colored_vg.setInputCloud(cloud);
	colored_vg.setLeafSize(0.01f, 0.01f, 0.01f);
	colored_vg.filter(*res);
	cout << "bleh" << endl;


	// Create a KD-Tree
	//pcl::search::KdTree<PointT>::Ptr tree(new pcl::search::KdTree<PointT>);

	//pcl::MovingLeastSquares<PointT, PointNormalT> mls;
	//mls.setComputeNormals(true);

	//// Set parameters
	//mls.setInputCloud(cloud);
	//mls.setPolynomialFit(true);
	//mls.setSearchMethod(tree);
	//mls.setSearchRadius(0.03);

	//// Reconstruct
	//mls.process(*res);

	//pcl::BilateralFilter<pcl::PointXYZI> fbFilter;
	//fbFilter.setInputCloud(cloud);
	//fbFilter.setHalfSize(1.0);
	//fbFilter.setStdDev(0.2);
	//fbFilter.applyFilter(cloud_filtered);

	cout << "PointCloud after filtering: " << res->width * res->height
		<< " data points (" << pcl::getFieldsList(*res) << ")." << endl;

	// Write filtered object
	/*	pcl::PLYWriter writer;
	writer.write("table_scene_lms400_downsampled.pcd", *cloud_filtered,
	Eigen::Vector4f::Zero(), Eigen::Quaternionf::Identity(), false); */
}


// NO COLOR VERSION
void reduce_size(PointCloud::Ptr cloud, PointCloud::Ptr res) {
	cout << "PointCloud without color before filtering: " << cloud->width * cloud->height
		<< " data points (" << pcl::getFieldsList(*cloud) << ")." << endl;

	/*PointCloud::Ptr temp(new PointCloud);

	pcl::StatisticalOutlierRemoval<PointT> sor;
	sor.setInputCloud(cloud);
	sor.setMeanK(50);
	sor.setStddevMulThresh(1.0);
	sor.filter(*temp);*/

	// Create the filtering object
	/*pcl::VoxelGrid<PointT> vg;
	vg.setInputCloud(cloud);
	vg.setLeafSize(0.01f, 0.01f, 0.01f);
	vg.filter(*res)*/;

	int _w, _h;
	_w = cloud->width;
	_h = cloud->height;

	cloud->width = 640;
	cloud->height = 480;
	std::cout << cloud->width << " ; " << cloud->height << "->ORG ? " << cloud->isOrganized() << std::endl;

	pcl::FastBilateralFilter<PointT> bilateral_filter;
	bilateral_filter.setInputCloud(cloud);
	bilateral_filter.setSigmaS(5);
	bilateral_filter.setSigmaR(1.0f);
	bilateral_filter.filter(*res);
	// --- cheat  ----
	//*res = *cloud;
	// ---------------
	res->width = _w;
	res->height = _h;
	cloud->width = _w;
	cloud->height = _h;
	std::cout << "DONE!" << std::endl;

	cout << "PointCloud after filtering: " << res->width * res->height
		<< " data points (" << pcl::getFieldsList(*res) << ")." << endl;

	cout << "Find " << res->points.size() << " points " << std::endl;

}

