#include <iostream>
#include <pcl/io/ply_io.h>
#include <pcl/point_types.h>

#include <pcl/registration/ndt.h>
#include <pcl/filters/approximate_voxel_grid.h>

#include <pcl/visualization/pcl_visualizer.h>
#include <boost/thread/thread.hpp>

#include <pcl/registration/icp.h>
#include <pcl/registration/icp_nl.h>

#include "IOXML.h"
#include "PCLCore.h"

bool comparePoint(pcl::PointXYZ p1, pcl::PointXYZ p2) {
	if (p1.x != p2.x)
		return p1.x > p2.x;
	else if (p1.y != p2.y)
		return  p1.y > p2.y;
	else
		return p1.z > p2.z;
}

bool equalPoint(pcl::PointXYZ p1, pcl::PointXYZ p2) {
	if (p1.x == p2.x && p1.y == p2.y && p1.z == p2.z)
		return true;
	return false;
}

int
main(int argc, char** argv)
{

	Eigen::Matrix4f * matrix_0 = PCLCore::getMatrix(std::string("matrix_0.xml"));
	Eigen::Matrix4f * matrix_1 = PCLCore::getMatrix(std::string("matrix_1.xml"));
	Eigen::Matrix4f * matrix_2 = PCLCore::getMatrix(std::string("matrix_2.xml"));
	
	std::cout << *matrix_0 << std::endl;
	std::cout << "---------------" << std::endl;
	std::cout << *matrix_1 << std::endl;
	std::cout << "---------------" << std::endl;
	std::cout << *matrix_2 << std::endl;

	// Loading first scan of room.
	pcl::PointCloud<pcl::PointXYZ>::Ptr target_cloud(new pcl::PointCloud<pcl::PointXYZ>);
	if (pcl::io::loadPLYFile<pcl::PointXYZ>("p1_filtered.ply", *target_cloud) == -1)
	{
		PCL_ERROR("Couldn't read file room_scan1.pcd \n");
		return (-1);
	}
	std::cout << "Loaded " << target_cloud->size() << " data points from p1" << std::endl;

	// Loading second scan of room from new perspective.
	pcl::PointCloud<pcl::PointXYZ>::Ptr input_cloud(new pcl::PointCloud<pcl::PointXYZ>);
	if (pcl::io::loadPLYFile<pcl::PointXYZ>("p2_filtered.ply", *input_cloud) == -1)
	{
		PCL_ERROR("Couldn't read file room_scan2.pcd \n");
		return (-1);
	}
	std::cout << "Loaded " << input_cloud->size() << " data points from p2" << std::endl;

	// removing useless points
	std::cout << "removing useless points" << std::endl;
	std::sort(target_cloud->points.begin(), target_cloud->points.end(), comparePoint);
	target_cloud->points.erase(std::unique(target_cloud->points.begin(), target_cloud->points.end(), equalPoint), target_cloud->points.end());
	target_cloud->width = target_cloud->points.size();
	pcl::io::savePLYFile("p1_filtered.ply", *target_cloud);
	std::cout << "p1 is now " << target_cloud->size() << std::endl;
	
	std::cout << "removing useless points" << std::endl;
	std::sort(input_cloud->points.begin(), input_cloud->points.end(), comparePoint);
	input_cloud->points.erase(std::unique(input_cloud->points.begin(), input_cloud->points.end(), equalPoint), input_cloud->points.end());
	input_cloud->width = input_cloud->points.size();
	pcl::io::savePLYFile("p2_filtered.ply", *input_cloud);
	std::cout << "p2 is now " << input_cloud->size() << std::endl;

	std::cout << "transform matrix" << std::endl;
	pcl::PointCloud<pcl::PointXYZ>::Ptr input_transformed(new pcl::PointCloud<pcl::PointXYZ>);
	pcl::PointCloud<pcl::PointXYZ>::Ptr target_transformed(new pcl::PointCloud<pcl::PointXYZ>);
	pcl::transformPointCloud(*target_cloud, *target_transformed, *matrix_0);
	Eigen::Matrix4f move = (matrix_1->inverse() * *(matrix_0));
	pcl::transformPointCloud(*input_cloud, *input_transformed, move);
	std::cout << "transformation done ! " << std::endl;

	pcl::PointCloud<pcl::PointXYZ>::Ptr display1(new pcl::PointCloud<pcl::PointXYZ>);
	*(display1) = *(input_transformed)+*(target_transformed);

	boost::shared_ptr<pcl::visualization::PCLVisualizer> v1(new pcl::visualization::PCLVisualizer("3D Viewer"));
	v1->setBackgroundColor(0, 0, 0);

	v1->addPointCloud(display1);
	// Wait until visualizer window is closed.
	while (!v1->wasStopped())
	{
		v1->spinOnce(100);
		boost::this_thread::sleep(boost::posix_time::microseconds(100000));
	}


	// Filtering input scan to roughly 10% of original size to increase speed of registration.
	pcl::PointCloud<pcl::PointXYZ>::Ptr filtered_input_cloud(new pcl::PointCloud<pcl::PointXYZ>);
	pcl::PointCloud<pcl::PointXYZ>::Ptr filtered_target_cloud(new pcl::PointCloud<pcl::PointXYZ>);
	pcl::ApproximateVoxelGrid<pcl::PointXYZ> approximate_voxel_filter;
	
	float leaf = 0.005f;
	approximate_voxel_filter.setLeafSize(leaf, leaf, leaf);
	approximate_voxel_filter.setInputCloud(input_transformed);
	approximate_voxel_filter.filter(*filtered_input_cloud);

	std::cout << "Filtered input contains " << filtered_input_cloud->size() << " points" << std::endl;
	approximate_voxel_filter.setInputCloud(target_transformed);
	approximate_voxel_filter.filter(*filtered_target_cloud);
	std::cout << "Filtered target contains " << filtered_target_cloud->size() << " points" << std::endl;

	

	// Initializing Normal Distributions Transform (NDT).
	pcl::NormalDistributionsTransform<pcl::PointXYZ, pcl::PointXYZ> ndt;

	// Setting scale dependent NDT parameters
	// Setting minimum transformation difference for termination condition.
	ndt.setTransformationEpsilon(0.01); // default 0.01
	// Setting maximum step size for Moore-Thuente line search.
	ndt.setStepSize(0.1); // default 0.1
	//Setting Resolution of NDT grid structure (VoxelGridCovariance).
	ndt.setResolution(0.1); // default 0.1

	// Setting max number of registration iterations.
	ndt.setMaximumIterations(50); // default 35

	// Setting point cloud to be aligned.
	ndt.setInputSource(filtered_input_cloud);
	// Setting point cloud to be aligned to.
	//ndt.setInputTarget(target_cloud);
	ndt.setInputTarget(filtered_target_cloud);
	std::cout << "NDT about to start" << std::endl;

	// Calculating required rigid transform to align the input cloud to the target cloud.
	pcl::PointCloud<pcl::PointXYZ>::Ptr output_cloud(new pcl::PointCloud<pcl::PointXYZ>);
	ndt.align(*output_cloud);
	

	std::cout << "Normal Distributions Transform has converged:" << ndt.hasConverged() << " score: " << ndt.getFitnessScore() << std::endl;
	std::cout << "In " << ndt.getFinalNumIteration() << " iterations " << std::endl;
	// Transforming unfiltered, input cloud using found transform.
	pcl::transformPointCloud(*input_cloud, *output_cloud, ndt.getFinalTransformation());

	std::cout << "transformation done" << std::endl;

	pcl::PointCloud<pcl::PointXYZ>::Ptr display(new pcl::PointCloud<pcl::PointXYZ>);
	*(display) = *(output_cloud) + *(target_cloud);

	// Initializing point cloud visualizer
	boost::shared_ptr<pcl::visualization::PCLVisualizer> viewer_final1(new pcl::visualization::PCLVisualizer("3D Viewer"));
	viewer_final1->setBackgroundColor(0, 0, 0);

	viewer_final1->addPointCloud(display);
	// Wait until visualizer window is closed.
	while (!viewer_final1->wasStopped())
	{
		viewer_final1->spinOnce(100);
		boost::this_thread::sleep(boost::posix_time::microseconds(100000));
	}

	std::cout << "Deleting temporary point cloud" << std::endl;
	//delete &display;

	// final cloud object
	pcl::PointCloud<pcl::PointXYZ>::Ptr final_cloud(new pcl::PointCloud<pcl::PointXYZ>);


	pcl::PointCloud<pcl::PointXYZ>::Ptr filtered_output_cloud(new pcl::PointCloud<pcl::PointXYZ>);
	approximate_voxel_filter.setInputCloud(output_cloud);
	approximate_voxel_filter.filter(*filtered_output_cloud);
	std::cout << "Filtered output contains " << filtered_output_cloud->size() << " points" << std::endl;
	// ICP objetct.
	//pcl::IterativeClosestPoint<pcl::PointXYZ, pcl::PointXYZ> registration;
	pcl::IterativeClosestPointNonLinear<pcl::PointXYZ, pcl::PointXYZ> registration;
	// ------------------------
	registration.setUseReciprocalCorrespondences(false);
	// ------------------------
	registration.setMaxCorrespondenceDistance(0.05);
	registration.setMaximumIterations(100);
	registration.setTransformationEpsilon(1e-8);
	registration.setInputSource(filtered_output_cloud);
	registration.setInputTarget(filtered_target_cloud);

	std::cout << "ICP starting " << std::endl;
	registration.align(*final_cloud);
	std::cout << "ICP done score : " << registration.getFitnessScore() << std::endl;
	std::cout << "In " << registration.getConvergeCriteria() << " iterations " << std::endl;
	pcl::transformPointCloud(*output_cloud, *final_cloud, registration.getFinalTransformation());

	std::cout << "transformation done" << std::endl;

	std::cout << "Merging point cloud" << std::endl;
	*(final_cloud) += *(target_cloud);

	std::cout << "Merge done" << std::endl;
	// Saving transformed input cloud.
	//pcl::io::savePLYFile("p1p2_transformed.ply", *final_cloud);

	//std::cout << "Save done" << std::endl;

	// Initializing point cloud visualizer
	boost::shared_ptr<pcl::visualization::PCLVisualizer> viewer_final(new pcl::visualization::PCLVisualizer("3D Viewer"));
	viewer_final->setBackgroundColor(0, 0, 0);

	viewer_final->addPointCloud(final_cloud);
	// Wait until visualizer window is closed.
	while (!viewer_final->wasStopped())
	{
		viewer_final->spinOnce(100);
		boost::this_thread::sleep(boost::posix_time::microseconds(100000));
	}

	return (0);
}