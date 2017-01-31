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
//#include <pcl/surface/simplification_remove_unused_vertices.h>
//
//#include <pcl/filters/approximate_voxel_grid.h>
//#include <pcl/filters/voxel_grid.h>
//
//
//#include <pcl/registration/icp.h>
//using namespace std;
//
//pcl::visualization::PCLVisualizer::Ptr viewer;
//
//
//bool comparePoint(pcl::PointXYZ p1, pcl::PointXYZ p2) {
//	if (p1.x != p2.x)
//		return p1.x > p2.x;
//	else if (p1.y != p2.y)
//		return  p1.y > p2.y;
//	else
//		return p1.z > p2.z;
//}
//
//bool equalPoint(pcl::PointXYZ p1, pcl::PointXYZ p2) {
//	if (p1.x == p2.x && p1.y == p2.y && p1.z == p2.z)
//		return true;
//	return false;
//}
//
//void reduce(pcl::PolygonMesh::Ptr cloud, bool save, const char * filename) {
//	cout << "reducing " << endl;
//	pcl::PointCloud<pcl::PointXYZ>::Ptr src_cloud(new pcl::PointCloud<pcl::PointXYZ>()); // = src->cloud;
//	pcl::fromROSMsg(cloud->cloud, *src_cloud);
//	
//	std::sort(src_cloud->points.begin(), src_cloud->points.end(), comparePoint);
//	src_cloud->points.erase(std::unique(src_cloud->points.begin(), src_cloud->points.end(), equalPoint), src_cloud->points.end());
//	cout << "reduced " << src_cloud->points.size() << endl;
//	cout << "w: " << src_cloud->width << " ; h: " << src_cloud->height << endl;
//	src_cloud->width = src_cloud->points.size();
//
//	pcl::PolygonMesh visible(*cloud);
//	pcl::surface::SimplificationRemoveUnusedVertices cleaner;
//	cleaner.simplify(visible, *cloud);
//	pcl::toROSMsg(*src_cloud, cloud->cloud);
//
//	if (save) {
//
//		IOPLY::save(filename, cloud);
//		cout << filename << " saved !" << endl;
//	}
//}
//
//void registration(pcl::PolygonMesh::Ptr src, pcl::PolygonMesh::Ptr tgt, bool display) {
//
//	//reduce(src, true, "p1-light.ply");
//	//reduce(tgt, true, "p2-light.ply");
//
//
//	pcl::PointCloud<pcl::PointXYZ>::Ptr src_cloud(new pcl::PointCloud<pcl::PointXYZ>()); // = src->cloud;
//	pcl::fromROSMsg(src->cloud, *src_cloud);
//	pcl::PointCloud<pcl::PointXYZ>::Ptr tgt_cloud(new pcl::PointCloud<pcl::PointXYZ>()); // = src->cloud;
//	pcl::fromROSMsg(tgt->cloud, *tgt_cloud);
//
//	pcl::PointCloud<pcl::PointXYZ>::Ptr filtered1(new pcl::PointCloud<pcl::PointXYZ>);
//	pcl::PointCloud<pcl::PointXYZ>::Ptr filtered2(new pcl::PointCloud<pcl::PointXYZ>);
//	pcl::PointCloud<pcl::PointXYZ>::Ptr transformed(new pcl::PointCloud<pcl::PointXYZ>);
//
//
//	// ICP objetct.
//	pcl::IterativeClosestPoint<pcl::PointXYZ, pcl::PointXYZ> registration;
//
//	cout << "-----------------------------------" << endl;
//
//	
//	registration.setMaxCorrespondenceDistance(0.05);
//	registration.setMaximumIterations(100);
//	registration.setTransformationEpsilon(1e-8);
//
//	pcl::VoxelGrid<PointT> avg;
//	// keeps only 1 voxel (point) within a sphere of 1cm radius
//	avg.setLeafSize(0.01, 0.01, 0.01);
//	avg.setInputCloud(tgt_cloud);
//	avg.filter(*filtered2);
//	cout << "tgt filtered " << filtered2->size() << "pts" << endl;
//
//	avg.setLeafSize(0.05, 0.05, 0.05);
//	
//	pcl::PointCloud<pcl::PointXYZ>::Ptr result(new pcl::PointCloud<pcl::PointXYZ>());
//	for (int i = 0; i < 10; i++) {
//		avg.setInputCloud(src_cloud);
//		avg.filter(*filtered1);
//		cout << "src filtered " << filtered1->size() << "pts" <<  endl;
//		
//
//		cout << "---------------------------------" << endl;
//		registration.setInputSource(filtered1);
//		registration.setInputTarget(filtered2);
//
//		Eigen::Matrix4f transformation;
//		registration.align(*result);
//		if (registration.hasConverged())
//		{
//			std::cout << "ICP converged." << std::endl
//				<< "The score is " << registration.getFitnessScore() << std::endl;
//			std::cout << "Transformation matrix:" << std::endl;
//			std::cout << registration.getFinalTransformation() << std::endl;
//			transformation = registration.getFinalTransformation();
//		}
//		else std::cout << "ICP did not converge." << std::endl;
//
//		//pcl::PointCloud<pcl::PointXYZ>::Ptr targetCloud(new pcl::PointCloud<pcl::PointXYZ>);
//		pcl::transformPointCloud(*src_cloud, *transformed, transformation);
//		src_cloud->points = transformed->points;
//		result->clear();
//		(*result) = (*transformed) + (*tgt_cloud);
//		if (display) {
//
//			viewer.reset(new pcl::visualization::PCLVisualizer);
//			viewer->removeAllPointClouds();
//			viewer->addPointCloud(result, "nuage");
//			viewer->spin();
//			//viewer->setShapeRenderingProperties(pcl::visualization::PCL_VISUALIZER_SHADING, pcl::visualization::PCL_VISUALIZER_SHADING_PHONG, "polygon");
//			//viewer->removeAllPointClouds();
//			//viewer->spin();
//		}
//		cout << "clearing..." << endl;
//		//filtered1->clear();
//		//filtered2->clear();
//		//result->clear();
//		//src->cloud.data.clear();
//		//tgt->cloud.data.clear();
//		//transformed->clear();
//
//		cout << "----------------" << endl;
//	}
//}
//
//int main(int argc, char** argv) {
//	string oops;
//
//	//pcl::ApproximateVoxelGrid<PointT> avg;
//	// keeps only 1 voxel (point) within a sphere of 1cm radius
//	//avg.setLeafSize(0.05, 0.05, 0.05);
//	// Objects for storing the point clouds.
//	//pcl::PointCloud<pcl::PointXYZ>::Ptr sourceCloud(new pcl::PointCloud<pcl::PointXYZ>);
//	//pcl::PointCloud<pcl::PointXYZ>::Ptr targetCloud(new pcl::PointCloud<pcl::PointXYZ>);
//	pcl::PolygonMesh::Ptr source(new pcl::PolygonMesh);
//	pcl::PolygonMesh::Ptr target(new pcl::PolygonMesh);
//	pcl::PointCloud<pcl::PointXYZ>::Ptr finalCloud(new pcl::PointCloud<pcl::PointXYZ>);
//
//	IOPLY::load("p1.ply", source);
//	cout << "file 1 loaded" << endl;
//	//cout << source->polygons.size() << endl;
//	//cout << source->cloud.data.size() << endl;
//	viewer.reset(new pcl::visualization::PCLVisualizer);
//	//viewer->removeAllPointClouds();
//	viewer->addPolygonMesh(*source);
//	viewer->spin();
//	//reduce(source, true, "p1-light2.ply");
//	//viewer.reset(new pcl::visualization::PCLVisualizer);
//	//viewer->removeAllPointClouds();
//	//viewer->addPointCloud(source->cloud);
//	//viewer->spin();
//	//cout << "reduced p1 has " << source->cloud.data.size() << " points " << endl;
//	// Sort and remove duplicate indices
//	//std::sort(sourceCloud->points.begin(), sourceCloud->points.end(), comparePoint);
//	//sourceCloud->points.erase(std::unique(sourceCloud->points.begin(), sourceCloud->points.end(), equalPoint), sourceCloud->points.end());
//	//cout << "after  p1 has " << sourceCloud->points.size() << " points " << endl;
//	//sourceCloud->width = sourceCloud->points.size();
//	//cout << "p1 " << sourceCloud->points.size() << " << " << sourceCloud->width << "*" << sourceCloud->height << endl;
//	//cout << "writing to file" << endl;
//	//IOPLY::save("p1-light.ply", sourceCloud);
//	//cout << "saved ! " << endl;
//	//cout << "filter p1 has " << filtered1->points.size() << " points" << endl;
//	//cout << "------------------------------" << endl;
//	IOPLY::load("p2.ply", target);
//	//reduce(target, true, "p2-light.ply");
//	cout << "file 2 loaded" << endl;
//	//cout << "before p2 has " << target->cloud.data.size() << " points " << endl;
//	// Sort and remove duplicate indices
//	//std::sort(targetCloud->points.begin(), targetCloud->points.end(), comparePoint);
//	//targetCloud->points.erase(std::unique(targetCloud->points.begin(), targetCloud->points.end(), equalPoint), targetCloud->points.end());
//	//cout << "after  p2 has " << targetCloud->points.size() << " points " << endl;
//
//	//targetCloud->width = targetCloud->points.size();
//	//cout << "writing to file" << endl;
//	//IOPLY::save("p2-light.ply", targetCloud);
//	//cout << "saved ! " << endl;
//	
//	//avg.setInputCloud(targetCloud);
//	//avg.filter(*filtered2);
//	//cout << "filter p1 has " << filtered2->points.size() << " points" << endl;
//	//cout << "------------------------------" << endl;
//	
//	registration(source, target, true);
//
//	/*
//	
//	viewer.reset(new pcl::visualization::PCLVisualizer);
//	viewer->addPointCloud(finalCloud);
//	viewer->spin();
//
//	*/
//
//	// Finish
//	return (0);
//}
