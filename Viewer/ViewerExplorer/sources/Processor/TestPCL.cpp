<<<<<<< cd35207e91adf00f735590971c22fb55e2d0a713
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
=======
#include <iostream>
#include <pcl/visualization/pcl_visualizer.h>
#include <pcl/point_types.h>
#include <IOPLY.h>


typedef struct {
	// structure used to pass arguments to the callback function
	const char* id;
	PointCloud::Ptr clicked_points_3d;
	Visualizer::Ptr viewerPtr;
	// MainWindow * ui;
} _args;

void pointPickingEventOccurred(const pcl::visualization::PointPickingEvent &event, void* args)
{
	_args* data = (_args*) args;
	pcl::console::print_info("bleh-1\n");

	// Max elem reached (6) don't add any other element
	if (data->clicked_points_3d->size() >= 6)
		return;

	pcl::PointXYZ point;

	event.getPoint(point.x, point.y, point.z);

	pcl::console::print_info("Added point : {x: %f, y: %f, z: %f}\n", point.x, point.y, point.z);

	data->clicked_points_3d->push_back(point);

	bool baseColor;
	PointCloud::Ptr tmpCloud(new PointCloud);

	// Update each colored point
	for (int i = 0; i < data->clicked_points_3d->size(); i++) {
		data->viewerPtr->removePointCloud(data->id + i);
		pcl::console::print_info("bleh1 : %i\n", data->id + i);
		baseColor = (i / 3 == 0);
		tmpCloud->push_back(data->clicked_points_3d->at(i));
		pcl::console::print_info("bleeeh1.5 : %b\n", baseColor);
		pcl::console::print_info("bleeeh1.75 : %i\n", i % 3);
		data->viewerPtr->addPointCloud(tmpCloud, pcl::visualization::PointCloudColorHandlerCustom<PointT>(data->clicked_points_3d,
			(i % 3) == 0 ? (baseColor ? 210 : 50) : (baseColor ? 50 : 210),
			(i % 3) == 1 ? (baseColor ? 210 : 50) : (baseColor ? 50 : 210),
			(i % 3) == 2 ? (baseColor ? 210 : 50) : (baseColor ? 50 : 210)), data->id + i);
		pcl::console::print_info("bleh2\n");
		data->viewerPtr->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 13, data->id + i);
		pcl::console::print_info("bleh3\n");
		tmpCloud->clear();
	}
}

int main(int argc, char** argv)
{
	std::cout << "ET TA MERE LA TEPU TAFFICHE QQCH OU BIEN ????" << std::endl;

	const char* f1 = "test/plan1-removed-duplicated-closemerged.ply";
	const char* f2 = "test/plan2-removed-duplicated-closemerged.ply";

	pcl::PolygonMesh::Ptr src(new pcl::PolygonMesh());
	pcl::PolygonMesh::Ptr tgt(new pcl::PolygonMesh());
	pcl::console::print_highlight("----- Loading1 -----\n");
	IOPLY::load(f1, src);
	pcl::console::print_highlight("----- Loading2 -----\n");
	IOPLY::load(f2, tgt);

	pcl::console::print_highlight("----- Loaded -----\n");

	pcl::PointCloud<pcl::PointXYZ>::Ptr src_pick(new pcl::PointCloud<pcl::PointXYZ>);
	pcl::PointCloud<pcl::PointXYZ>::Ptr tgt_pick(new pcl::PointCloud<pcl::PointXYZ>);

	pcl::visualization::PCLVisualizer::Ptr viewer1(new pcl::visualization::PCLVisualizer);
	pcl::visualization::PCLVisualizer::Ptr viewer2(new pcl::visualization::PCLVisualizer);

	viewer1->addPolygonMesh(*src, "src");
	viewer2->addPolygonMesh(*tgt, "tgt");

	// ----- start color stuff -----
	// src
	std::vector<pcl::visualization::PointCloudColorHandlerCustom<pcl::PointXYZ>> src_colors;
	pcl::visualization::PointCloudColorHandlerCustom<pcl::PointXYZ> src_light_red(src_pick, 210, 50, 50); // light red
	pcl::visualization::PointCloudColorHandlerCustom<pcl::PointXYZ> src_light_green(tgt_pick, 50, 210, 50); // light green
	pcl::visualization::PointCloudColorHandlerCustom<pcl::PointXYZ> src_light_blue(src_pick, 50, 50, 210); // light red
	pcl::visualization::PointCloudColorHandlerCustom<pcl::PointXYZ> src_light_cyan(tgt_pick, 50, 210, 210); // light green
	pcl::visualization::PointCloudColorHandlerCustom<pcl::PointXYZ> src_light_violet(src_pick, 210, 50, 210); // light red
	pcl::visualization::PointCloudColorHandlerCustom<pcl::PointXYZ> src_light_orange(tgt_pick, 210, 210, 50); // light green
	src_colors.push_back(src_light_red);
	src_colors.push_back(src_light_green);
	src_colors.push_back(src_light_blue);
	src_colors.push_back(src_light_cyan);
	src_colors.push_back(src_light_violet);
	src_colors.push_back(src_light_orange);
	// tgt
	std::vector<pcl::visualization::PointCloudColorHandlerCustom<pcl::PointXYZ>> tgt_colors;
	pcl::visualization::PointCloudColorHandlerCustom<pcl::PointXYZ> tgt_light_red(src_pick, 210, 50, 50); // light red
	pcl::visualization::PointCloudColorHandlerCustom<pcl::PointXYZ> tgt_light_green(tgt_pick, 50, 210, 50); // light green
	pcl::visualization::PointCloudColorHandlerCustom<pcl::PointXYZ> tgt_light_blue(src_pick, 50, 50, 210); // light red
	pcl::visualization::PointCloudColorHandlerCustom<pcl::PointXYZ> tgt_light_cyan(tgt_pick, 50, 210, 210); // light green
	pcl::visualization::PointCloudColorHandlerCustom<pcl::PointXYZ> tgt_light_violet(src_pick, 210, 50, 210); // light red
	pcl::visualization::PointCloudColorHandlerCustom<pcl::PointXYZ> tgt_light_orange(tgt_pick, 210, 210, 50); // light green
	tgt_colors.push_back(tgt_light_red);
	tgt_colors.push_back(tgt_light_green);
	tgt_colors.push_back(tgt_light_blue);
	tgt_colors.push_back(tgt_light_cyan);
	tgt_colors.push_back(tgt_light_violet);
	tgt_colors.push_back(tgt_light_orange);
	// ----- end color stuff -----

	// ----- igit -----
	viewer1->addPointCloud(src_pick, src_colors.at(0), "src_pick0");
	viewer1->addPointCloud(src_pick, src_colors.at(0), "src_pick1");
	viewer1->addPointCloud(src_pick, src_colors.at(0), "src_pick2");
	viewer1->addPointCloud(src_pick, src_colors.at(0), "src_pick3");
	viewer1->addPointCloud(src_pick, src_colors.at(0), "src_pick4");
	viewer1->addPointCloud(src_pick, src_colors.at(0), "src_pick5");
	viewer1->addPointCloud(src_pick, src_colors.at(0), "src_pick6");
	viewer2->addPointCloud(tgt_pick, tgt_colors.at(0), "tgt_pick0");
	viewer2->addPointCloud(tgt_pick, tgt_colors.at(0), "tgt_pick1");
	viewer2->addPointCloud(tgt_pick, tgt_colors.at(0), "tgt_pick2");
	viewer2->addPointCloud(tgt_pick, tgt_colors.at(0), "tgt_pick3");
	viewer2->addPointCloud(tgt_pick, tgt_colors.at(0), "tgt_pick4");
	viewer2->addPointCloud(tgt_pick, tgt_colors.at(0), "tgt_pick5");
	viewer2->addPointCloud(tgt_pick, tgt_colors.at(0), "tgt_pick6");

	viewer1->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 13, "src_pick0");
	viewer1->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 13, "src_pick1");
	viewer1->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 13, "src_pick2");
	viewer1->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 13, "src_pick3");
	viewer1->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 13, "src_pick4");
	viewer1->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 13, "src_pick5");
	viewer1->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 13, "src_pick6");
	viewer2->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 13, "tgt_pick0");
	viewer2->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 13, "tgt_pick1");
	viewer2->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 13, "tgt_pick2");
	viewer2->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 13, "tgt_pick3");
	viewer2->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 13, "tgt_pick4");
	viewer2->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 13, "tgt_pick5");
	viewer2->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 13, "tgt_pick6");
	// ----- igit -----

	_args src_args;
	src_args.id = "src_pick";
	src_args.clicked_points_3d = src_pick;
	src_args.viewerPtr = viewer1;
	_args tgt_args;
	tgt_args.id = "tgt_pick";
	tgt_args.clicked_points_3d = tgt_pick;
	tgt_args.viewerPtr = viewer2;

	viewer1->registerPointPickingCallback(pointPickingEventOccurred, (void*)&src_args);
	viewer2->registerPointPickingCallback(pointPickingEventOccurred, (void*)&tgt_args);

	viewer1->spin();
	viewer2->spin();
}
>>>>>>> 2ae6e75bd1f8310b6096e42a900233730e5d8419
