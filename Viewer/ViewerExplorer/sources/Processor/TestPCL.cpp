////#include "stdafx.h"
//
//
//#pragma warning( disable : 4996 )
//#include <iostream>
//#include <string>
//#include "custom_typedef.h"
//#include <pcl/point_types.h>
//#include "IOPLY.h"
//#include <pcl/kdtree/kdtree_flann.h>
//#include <pcl/features/normal_3d.h>
//#include <pcl/surface/gp3.h>
//#include <pcl/io/vtk_lib_io.h>
//#include <pcl/surface/mls.h>
//
//#include <pcl/filters/approximate_voxel_grid.h>
//using namespace std;
//
//pcl::visualization::PCLVisualizer::Ptr viewer;
//
//int main(int argc, char** argv) {
//	// Load input file into a PointCloud<T> with an appropriate type
//	pcl::PointCloud<pcl::PointXYZ>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZ>);
//	pcl::PCLPointCloud2 cloud_blob;
//	pcl::io::loadPLYFile("lit_mur1.ply", cloud_blob);
//	std::cout << "loaded : " << cloud_blob.data.size() << " : " << cloud->points.size() << std::endl;
//	pcl::fromPCLPointCloud2(cloud_blob, *cloud);
//	std::cout << "Conversion to PointCloud2 over" << std::endl;
//
//	std::cout << "MLS about to start" << std::endl;
//
//	// Create a KD-Tree
//	pcl::search::KdTree<pcl::PointXYZ>::Ptr tree(new pcl::search::KdTree<pcl::PointXYZ>);
//
//	// Output has the PointNormal type in order to store the normals calculated by MLS
//	pcl::PointCloud<pcl::PointNormal> mls_points;
//
//	// Init object (second point type is for the normals, even if unused)
//	pcl::MovingLeastSquares<pcl::PointXYZ, pcl::PointNormal> mls;
//
//	mls.setComputeNormals(true);
//
//	// Set parameters
//	mls.setInputCloud(cloud);
//	mls.setPolynomialFit(true);
//	mls.setSearchMethod(tree);
//	mls.setSearchRadius(0.03);
//
//	// Reconstruct
//	mls.process(mls_points);
//
//	std::cout << "MLS over" << std::endl;
//	std::cout << "MLS Conversion" << std::endl;
//
//	pcl::PointCloud<pcl::PointXYZ>::Ptr mls_cloud(new pcl::PointCloud<pcl::PointXYZ>);
//	mls_cloud->resize(mls_points.size());
//
//	for (size_t i = 0; i < mls_points.points.size(); ++i)
//	{
//		mls_cloud->points[i].x = mls_points.points[i].x; //error 
//		mls_cloud->points[i].y = mls_points.points[i].y; //error 
//		mls_cloud->points[i].z = mls_points.points[i].z; //error 
//	}
//	std::cout << "MLS Converted" << std::endl;
//
//	PointCloud::Ptr filtered_cloud(new PointCloud);
//
//	std::cout << "reducing size " << std::endl;
//	pcl::ApproximateVoxelGrid<PointT> avg;
//	// keeps only 1 voxel (point) within a sphere of 1cm radius
//	avg.setLeafSize(0.05, 0.05, 0.05);
//	avg.setInputCloud(mls_cloud);
//	avg.filter(*filtered_cloud);
//	std::cout << "reduce over : " << filtered_cloud->points.size() <<  std::endl;
//	
//	//* the data should be available in cloud
//	// Normal estimation*
//	pcl::NormalEstimation<pcl::PointXYZ, pcl::Normal> n;
//	pcl::PointCloud<pcl::Normal>::Ptr normals(new pcl::PointCloud<pcl::Normal>);
//	pcl::search::KdTree<pcl::PointXYZ>::Ptr tree2(new pcl::search::KdTree<pcl::PointXYZ>);
//	tree2->setInputCloud(filtered_cloud);
//	n.setInputCloud(filtered_cloud);
//	n.setSearchMethod(tree2);
//	n.setKSearch(20);
//	n.compute(*normals);
//	//* normals should not contain the point normals + surface curvatures
//
//	std::cout << "normal computation over " << std::endl;
//
//	// Concatenate the XYZ and normal fields*
//	pcl::PointCloud<pcl::PointNormal>::Ptr cloud_with_normals(new pcl::PointCloud<pcl::PointNormal>);
//	pcl::concatenateFields(*filtered_cloud, *normals, *cloud_with_normals);
//	//* cloud_with_normals = cloud + normals
//
//	// Create search tree*
//	pcl::search::KdTree<pcl::PointNormal>::Ptr tree3(new pcl::search::KdTree<pcl::PointNormal>);
//	tree3->setInputCloud(cloud_with_normals);
//
//	// Initialize objects
//	pcl::GreedyProjectionTriangulation<pcl::PointNormal> gp3;
//	pcl::PolygonMesh triangles;
//		
//	// Set the maximum distance between connected points (maximum edge length)
//	gp3.setSearchRadius(0.1);
//
//	// Set typical values for the parameters
//	gp3.setMu(2.5);
//	gp3.setMaximumNearestNeighbors(100);
//	gp3.setMaximumSurfaceAngle(M_PI / 4); // 45 degrees
//	gp3.setMinimumAngle(M_PI / 18); // 10 degrees
//	gp3.setMaximumAngle(2 * M_PI / 3); // 120 degrees
//	gp3.setNormalConsistency(false);
//
//	std::cout << "reconstruction about to start" << std::endl;
//	// Get result
//	gp3.setInputCloud(cloud_with_normals);
//	gp3.setSearchMethod(tree3);
//	gp3.reconstruct(triangles);
//	std::cout << "reconstruction over" << std::endl;
//
//	// Additional vertex information
//	std::vector<int> parts = gp3.getPartIDs();
//	std::vector<int> states = gp3.getPointStates();
//
//	std::cout << "parts size : " << parts.size() << std::endl;
//	std::cout << "states size : " << states.size() << std::endl;
//	//boost::shared_ptr<pcl::visualization::PCLVisualizer> viewer = *static_cast<boost::shared_ptr<pcl::visualization::PCLVisualizer> *> (viewer_void);
//	viewer.reset(new pcl::visualization::PCLVisualizer);
//	viewer->setShapeRenderingProperties(pcl::visualization::PCL_VISUALIZER_SHADING, pcl::visualization::PCL_VISUALIZER_SHADING_PHONG, "polygon");
//	viewer->addPolygonMesh(triangles, "polygon");
//	viewer->spin();
//	// Finish
//	return (0);
//}
