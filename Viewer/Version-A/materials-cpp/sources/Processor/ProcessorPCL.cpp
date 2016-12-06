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
//#include <pcl/visualization/pcl_visualizer.h>
#include <pcl/filters/bilateral.h>
#include <pcl/filters/impl/bilateral.hpp>
#include <pcl/filters/fast_bilateral.h>
#include <pcl/filters/statistical_outlier_removal.h>
#include <pcl/surface/mls.h>
#include <pcl/point_representation.h>

#include "custom_typedef.h"
#include "IOPLY.h"


#include "MetricVisualizer.h"

using pcl::visualization::PointCloudColorHandlerGenericField;
using pcl::visualization::PointCloudColorHandlerCustom;


unsigned int text_id = 0;
void mouseEventOccurred(const pcl::visualization::MouseEvent &event, void* viewer_void) {
	// retrieve visualizer
	boost::shared_ptr<pcl::visualization::PCLVisualizer> viewer = *static_cast<boost::shared_ptr<pcl::visualization::PCLVisualizer> *> (viewer_void);
	// if left mouse button clicked
	if (event.getButton() == pcl::visualization::MouseEvent::LeftButton && event.getType() == pcl::visualization::MouseEvent::MouseButtonRelease) {
		// debug
		std::cout << "Left mouse button released at position (" << event.getX() << ", " << event.getY() << ")" << std::endl;
		char str[512];
		// instanciate text
		sprintf(str, "text#%03d", text_id++);
		// show text
		viewer->addText("clicked here", event.getX(), event.getY(), str);
	}
}


//struct callback_args {
//	// structure used to pass arguments to the callback function
//	PointCloud::Ptr clicked_points_3d;
//	Visualizer::Ptr viewerPtr;
//	char * distance;
//};

//
//void pp_callback(const pcl::visualization::PointPickingEvent& event, void* args)
//{
//	// retrieve parameter
//	struct callback_args* data = (struct callback_args *)args;
//	// in case not point found, exit
//	if (event.getPointIndex() == -1)
//		return;
//
//	// instanciate point
//	PointT current_point;
//
//	// retrieve values
//	event.getPoint(current_point.x, current_point.y, current_point.z);
//
//	// if two points are already in the list
//	if (data->clicked_points_3d->points.size() == 2) {
//		// empty the list
//		data->clicked_points_3d->points.clear();
//		// reset distance value
//		sprintf(data->distance, "%f", 0);
//	}
//
//	// add point to the list
//	data->clicked_points_3d->points.push_back(current_point);
//
//	// if there are two points in the list
//	if (data->clicked_points_3d->points.size() == 2) {
//		double dist;
//		// retrieve values
//		PointT point1 = data->clicked_points_3d->points.at(0);
//		PointT point2 = data->clicked_points_3d->points.at(1);
//
//		// compute distance
//		dist = sqrt(pow(point1.x - point2.x, 2) + pow(point1.y - point2.y, 2) + pow(point1.z - point2.z, 2));
//		// set value
//		sprintf(data->distance, "%f", dist);
//		// debug it
//		std::cout << "DISTANCE : " << data->distance << std::endl;
//	}
//
//	// Draw clicked points in red:
//	pcl::visualization::PointCloudColorHandlerCustom<PointT> red(data->clicked_points_3d, 255, 0, 0);
//	data->viewerPtr->removePointCloud("clicked_points");
//	data->viewerPtr->addPointCloud(data->clicked_points_3d, red, "clicked_points");
//	data->viewerPtr->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 10, "clicked_points");
//	// debug 
//	std::cout << current_point.x << " " << current_point.y << " " << current_point.z << std::endl;
//}
//

void keyboardEventOccurred(const pcl::visualization::KeyboardEvent &event, void* viewer_void) {
	// retrieve visualizer
	boost::shared_ptr<pcl::visualization::PCLVisualizer> viewer = *static_cast<boost::shared_ptr<pcl::visualization::PCLVisualizer> *> (viewer_void);
	// if user press 'R'
	if (event.getKeySym() == "r" && event.keyDown()) {
		// debug
		std::cout << "r was pressed => removing all text" << std::endl;
		char str[512];

		// remove all added shapes
		for (unsigned int i = 0; i < text_id; ++i) {
			sprintf(str, "text#%03d", i);
			viewer->removeShape(str);
		}
		// reset ID
		text_id = 0;
	}
}

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

	cout << "Find " << res->points.size() << " points " << std::endl;

	// Write filtered object
	/*	pcl::PLYWriter writer;
	writer.write("table_scene_lms400_downsampled.pcd", *cloud_filtered,
	Eigen::Vector4f::Zero(), Eigen::Quaternionf::Identity(), false); */
}

// COLORED VERSION
double computeCloudResolution(const PointCloudColored::ConstPtr &cloud) {
	double res = 0.0;
	int n_points = 0;
	int nres;
	std::vector<int> indices(2);
	std::vector<float> sqr_distances(2);
	pcl::search::KdTree<PointColorT> tree;
	tree.setInputCloud(cloud);

	for (size_t i = 0; i < cloud->size(); ++i) {
		if (!pcl_isfinite((*cloud)[i].x)) {
			continue;
		}
		//Considering the second neighbor since the first is the point itself.
		nres = tree.nearestKSearch(i, 2, indices, sqr_distances);
		if (nres == 2) {
			res += sqrt(sqr_distances[1]);
			++n_points;
		}
	}

	if (n_points != 0) {
		res /= n_points;
	}

	return res;
}


// NO COLOR VERSION
double computeCloudResolution(const PointCloud::ConstPtr &cloud) {
	double res = 0.0;
	int n_points = 0;
	int nres;
	std::vector<int> indices(2);
	std::vector<float> sqr_distances(2);
	pcl::search::KdTree<PointT> tree;
	tree.setInputCloud(cloud);

	for (size_t i = 0; i < cloud->size(); ++i) {
		if (!pcl_isfinite((*cloud)[i].x)) {
			continue;
		}
		//Considering the second neighbor since the first is the point itself.
		nres = tree.nearestKSearch(i, 2, indices, sqr_distances);
		if (nres == 2) {
			res += sqrt(sqr_distances[1]);
			++n_points;
		}
	}

	if (n_points != 0) {
		res /= n_points;
	}

	return res;
}


// COLORED VERSION
void ISS(PointCloudColored::Ptr cloud, PointCloudColored::Ptr keypoints, IndicesT::Ptr indices) {
	cout << "Cloud with size = " << cloud->size() << endl;
	// ----- ISS keypoints -----
	pcl::ISSKeypoint3D<PointColorT, PointColorT> iss_detector;
	// ----- Cloud resolution -----
	double model_resolution = computeCloudResolution(cloud);
	// ISS parameters
	double gamma_21(0.975);
	double gamma_32(0.975);
	double min_neighbors(5);
	int threads(4);
	double salient_radius = 6 * model_resolution;
	double non_max_radius = 4 * model_resolution;
	// double normal_radius = 4 * model_resolution;
	// double border_radius = 1 * model_resolution;
	iss_detector.setSalientRadius(salient_radius);
	iss_detector.setNonMaxRadius(non_max_radius);
	// iss_detector.setNormalRadius(normal_radius);
	// iss_detector.setBorderRadius(border_radius);
	iss_detector.setThreshold21(gamma_21);
	iss_detector.setThreshold32(gamma_32);
	iss_detector.setMinNeighbors(min_neighbors);
	iss_detector.setNumberOfThreads(threads);
	iss_detector.setInputCloud(cloud);
	// ISS result
	iss_detector.compute(*keypoints);
	for (int i = 0; i < keypoints->size(); i++)
		indices->indices.push_back(iss_detector.getIndices()->at(i));
	cout << "Keypoints found : " << keypoints->size() << endl;
}

// NO COLOR VERSION
void ISS(PointCloud::Ptr cloud, PointCloud::Ptr keypoints, IndicesT::Ptr indices) {
	cout << "Cloud with size = " << cloud->size() << endl;
	// ----- ISS keypoints -----
	pcl::ISSKeypoint3D<PointT, PointT> iss_detector;
	// ----- Cloud resolution -----
	double model_resolution = computeCloudResolution(cloud);
	// ISS parameters
	double gamma_21(0.975);
	double gamma_32(0.975);
	double min_neighbors(5);
	int threads(4);
	double salient_radius = 6 * model_resolution;
	double non_max_radius = 4 * model_resolution;
	// double normal_radius = 4 * model_resolution;
	// double border_radius = 1 * model_resolution;
	iss_detector.setSalientRadius(salient_radius);
	iss_detector.setNonMaxRadius(non_max_radius);
	// iss_detector.setNormalRadius(normal_radius);
	// iss_detector.setBorderRadius(border_radius);
	iss_detector.setThreshold21(gamma_21);
	iss_detector.setThreshold32(gamma_32);
	iss_detector.setMinNeighbors(min_neighbors);
	iss_detector.setNumberOfThreads(threads);
	iss_detector.setInputCloud(cloud);
	// ISS result
	iss_detector.compute(*keypoints);
	for (int i = 0; i < keypoints->size(); i++)
		indices->indices.push_back(iss_detector.getIndices()->at(i));
	cout << "Keypoints found : " << keypoints->size() << endl;
}

void FPFH(PointCloud::Ptr cloud, PointCloudNormals::Ptr normals, PointCloud::Ptr keypoints, PointCloudFPFH::Ptr features) {
	cout << "Cloud with size = " << cloud->size() << endl;
	// ----- FPFH features -----
	pcl::FPFHEstimation<PointT, PointNormalT, FPFHSignature33T> fpfh;
	// FPFH parameters
	float radius(0.05);
	fpfh.setSearchSurface(cloud);
	fpfh.setInputNormals(normals);
	fpfh.setRadiusSearch(radius);
	fpfh.setInputCloud(keypoints);
	// FPFH result
	fpfh.compute(*features);
	cout << "Features found : " << features->size() << endl;
}


void registration_transformation_estimation(PointCloudFPFH::Ptr source_features, PointCloudFPFH::Ptr target_features,
	PointCloud::Ptr source_keypoints, PointCloud::Ptr target_keypoints,
	Eigen::Matrix4f *transform) {
	cout << "Registration" << endl;
	// ----- Correspondences -----
	pcl::CorrespondencesPtr correspondences(new pcl::Correspondences);
	pcl::registration::CorrespondenceEstimation<FPFHSignature33T, FPFHSignature33T> cest;
	// Correspondences parameters
	cest.setInputSource(source_features);
	cest.setInputTarget(target_features);
	// Correspondences result
	cest.determineCorrespondences(*correspondences);

	// ----- Rejector -----
	pcl::CorrespondencesPtr corr_filtered(new pcl::Correspondences);
	pcl::registration::CorrespondenceRejectorSampleConsensus<PointT> rejector;
	// Rejector parameters
	rejector.setInputSource(source_keypoints);
	rejector.setInputTarget(target_keypoints);
	rejector.setInlierThreshold(0.1);
	rejector.setMaximumIterations(1000);
	rejector.setRefineModel(true);
	rejector.setInputCorrespondences(correspondences);
	// Rejector result
	rejector.getCorrespondences(*corr_filtered);

	// ----- Transformation estimation -----
	pcl::registration::TransformationEstimationSVD<PointT, PointT> trans_est;
	trans_est.estimateRigidTransformation(*source_keypoints, *target_keypoints, *corr_filtered, *transform);
}


void normals(PointCloud::Ptr cloud, PointCloudNormals::Ptr normals) {
	cout << "Cloud with size = " << cloud->size() << endl;

	// ----- Normal estimation -----
	pcl::NormalEstimation<PointT, PointNormalT> ne;
	// Normal parameters
	ne.setInputCloud(cloud);
	// Tree search
	pcl::search::KdTree<PointT>::Ptr tree(new pcl::search::KdTree<PointT>());
	ne.setSearchMethod(tree);
	ne.setRadiusSearch(0.05);
	// Normal result
	ne.compute(*normals);
	pcl::concatenateFields(*normals, *cloud, *normals);

	cout << "normals found : " << normals->size() << endl;
}


// COLORED VERSION
void showClouds(PointCloudColored::Ptr src, PointCloudColored::Ptr tgt, PointCloudColored::Ptr res,
	PointCloudColored::Ptr src_keypoints, PointCloudNormals::Ptr src_normals,
	PointCloudColored::Ptr tgt_keypoints, PointCloudNormals::Ptr tgt_normals) {
	pcl::visualization::PCLVisualizer::Ptr v(new pcl::visualization::PCLVisualizer);

	int v1(1), v2(2), v3(3), v4(4);
	v->setShowFPS(true);
	// ----- FINAL RESULT -----
	v->createViewPort(0.0, 0.0, 0.5, 0.5, v1); // bot-left view
											   // Bot-left view content
	PointCloudColorHandlerCustom<PointColorT> src_rgb(src, 210, 50, 50); // Red
	PointCloudColorHandlerCustom<PointColorT> tgt_rgb(tgt, 50, 210, 50); // Green
	PointCloudColorHandlerCustom<PointColorT> res_rgb(res, 50, 50, 210); // Blue
	PointCloudColorHandlerCustom<PointColorT> keypoints_rgb(src, 210, 50, 210); // Yellow
	PointCloudColorHandlerCustom<PointColorT> normals_rgb(src, 190, 190, 190); // Snow white
	v->addPointCloud(src, src_rgb, "v1_source", v1);
	v->addPointCloud(tgt, tgt_rgb, "v1_target", v1);
	//v->addPointCloud(res, res_rgb, "v1_result", v1);
	// ----- SOURCE -----
	v->createViewPort(0.5, 0.0, 1.0, 0.5, v2); // bot-right view 
											   // Bot-right view content
	v->addPointCloud(src, src_rgb, "v2_source", v2);
	v->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 3, "v2_source", v2);
	v->addPointCloud(src_keypoints, keypoints_rgb, "v2_keypoints", v2);
	v->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 5, "v2_keypoints", v2);
	v->addPointCloudNormals<PointColorT, PointNormalT>(src, src_normals, 10, 0.05, "v2_normals", v2);
	// ----- TARGET -----
	v->createViewPort(0.0, 0.5, 0.5, 1.0, v3); // top-left view
											   // Top-left view content
	v->addPointCloud(tgt, tgt_rgb, "v3_target", v3);
	v->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 3, "v3_target", v3);
	v->addPointCloud(tgt_keypoints, keypoints_rgb, "v3_keypoints", v3);
	v->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 5, "v3_keypoints", v3);
	v->addPointCloudNormals<PointColorT, PointNormalT>(tgt, tgt_normals, 10, 0.05, "v3_normals", v3);
	// ----- RESULT -----
	v->createViewPort(0.5, 0.5, 1.0, 1.0, v4); // top-right view
											   // Top-right view content
	v->addPointCloud(src, src_rgb, "v4_source, v4");
	//v->addPointCloud(tgt, tgt_rgb, "v4_target, v4");
	v->addPointCloud(res, res_rgb, "v4_result", v4);

	// Spinning (drawing result)
	v->spin();
}

// NO COLOR VERSION
void showClouds(PointCloud::Ptr src, PointCloud::Ptr tgt, PointCloud::Ptr res,
	PointCloud::Ptr src_keypoints, PointCloudNormals::Ptr src_normals,
	PointCloud::Ptr tgt_keypoints, PointCloudNormals::Ptr tgt_normals) {
	pcl::visualization::PCLVisualizer::Ptr v(new pcl::visualization::PCLVisualizer);

	int v1(1), v2(2), v3(3), v4(4);
	v->setShowFPS(true);
	// ----- FINAL RESULT -----
	v->createViewPort(0.0, 0.0, 0.5, 0.5, v1); // bot-left view
											   // Bot-left view content
	PointCloudColorHandlerCustom<PointT> src_rgb(src, 210, 50, 50); // Red
	PointCloudColorHandlerCustom<PointT> tgt_rgb(tgt, 50, 210, 50); // Green
	PointCloudColorHandlerCustom<PointT> res_rgb(res, 50, 50, 210); // Blue
	PointCloudColorHandlerCustom<PointT> keypoints_rgb(src, 210, 50, 210); // Yellow
	PointCloudColorHandlerCustom<PointT> normals_rgb(src, 190, 190, 190); // Snow white
	v->addPointCloud(src, src_rgb, "v1_source", v1);
	v->addPointCloud(tgt, tgt_rgb, "v1_target", v1);
	//v->addPointCloud(res, res_rgb, "v1_result", v1);
	// ----- SOURCE -----
	v->createViewPort(0.5, 0.0, 1.0, 0.5, v2); // bot-right view 
											   // Bot-right view content
	v->addPointCloud(src, src_rgb, "v2_source", v2);
	v->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 3, "v2_source", v2);
	v->addPointCloud(src_keypoints, keypoints_rgb, "v2_keypoints", v2);
	v->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 5, "v2_keypoints", v2);
	v->addPointCloudNormals<PointT, PointNormalT>(src, src_normals, 10, 0.05, "v2_normals", v2);
	// ----- TARGET -----
	v->createViewPort(0.0, 0.5, 0.5, 1.0, v3); // top-left view
											   // Top-left view content
	v->addPointCloud(tgt, tgt_rgb, "v3_target", v3);
	v->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 3, "v3_target", v3);
	v->addPointCloud(tgt_keypoints, keypoints_rgb, "v3_keypoints", v3);
	v->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 5, "v3_keypoints", v3);
	v->addPointCloudNormals<PointT, PointNormalT>(tgt, tgt_normals, 10, 0.05, "v3_normals", v3);
	// ----- RESULT -----
	v->createViewPort(0.5, 0.5, 1.0, 1.0, v4); // top-right view
											   // Top-right view content
	v->addPointCloud(src, src_rgb, "v4_source, v4");
	//v->addPointCloud(tgt, tgt_rgb, "v4_target, v4");
	v->addPointCloud(res, res_rgb, "v4_result", v4);

	// Spinning (drawing result)
	v->spin();
}

bool once = true;
pcl::visualization::PCLVisualizer::Ptr pv(new pcl::visualization::PCLVisualizer);
int v(5);
void showIntermediateCloud(PointCloud::Ptr src, PointCloud::Ptr tgt, pcl::visualization::PCLVisualizer::Ptr pv) {
	if (once) {
		once = false;
		pv->setShowFPS(true);
		pv->createViewPort(0.0, 0.0, 1.0, 1.0, v);
	}
	else {
		pv->removeAllPointClouds(v);
	}
	// Content
	PointCloudColorHandlerCustom<PointT> src_rgb(src, 210, 50, 50); // Red
	PointCloudColorHandlerCustom<PointT> tgt_rgb(tgt, 50, 210, 50); // Green
	pv->addPointCloud(src, src_rgb, "v1_source", v);
	pv->addPointCloud(tgt, tgt_rgb, "v1_target", v);

	// Spinning (drawing result)
	pv->spinOnce(100);
}


// COLORED VERSION
void registration_ICP(PointCloudColored::Ptr src_corr, PointCloudColored::Ptr tgt_corr,
	PointCloudColored::Ptr res, float distance_max,
	Eigen::Matrix4f *transformation) {
	// Align
	pcl::IterativeClosestPoint<PointColorT, PointColorT> reg;
	reg.setTransformationEpsilon(1e-6);
	// Set the maximum distance between two correspondences (src<->tgt) to 10cm
	// Note: adjust this based on the size of your datasets
	reg.setMaxCorrespondenceDistance(3 * distance_max);
	reg.setRANSACOutlierRejectionThreshold(distance_max / 5);

	reg.setInputCloud(src_corr);
	reg.setInputTarget(tgt_corr);


	pcl::PointCloud<PointColorT> Final;
	reg.align(Final);

	cout << "there are " << Final.points.size() << " in final " << std::endl;

	std::cout << "conv ? " << reg.hasConverged() << ", score ? " << reg.getFitnessScore() << std::endl;
	std::cout << reg.getFinalTransformation() << std::endl;

	// Run the same optimization in a loop and visualize the results
	//Eigen::Matrix4f Ti = Eigen::Matrix4f::Identity(), prev;

	//reg.setMaximumIterations(1000);

	//// Get the transformation from target to source
	//Ti = reg.getFinalTransformation();
	//(*transformation) = Ti.inverse();
	//// Print matrix
	//for (int i = 0; i < 4; i++) {
	//	for (int j = 0; j < 4; j++)
	//		cout << (*transformation)(i, j) << " | ";
	//	cout << endl;
	//}
}

// NO COLOR VERSION
void registration_ICP(PointCloud::Ptr src_corr, PointCloud::Ptr tgt_corr,
	PointCloud::Ptr res, float distance_max,
	Eigen::Matrix4f *transformation) {
	// Align
	pcl::IterativeClosestPoint<PointT, PointT> reg;
	reg.setTransformationEpsilon(1e-6);
	// Set the maximum distance between two correspondences (src<->tgt) to 10cm
	// Note: adjust this based on the size of your datasets
	reg.setMaxCorrespondenceDistance(3 * distance_max);
	reg.setRANSACOutlierRejectionThreshold(distance_max / 5);
//	reg.setRANSACIterations(0);
	reg.setInputCloud(src_corr);
	reg.setInputTarget(tgt_corr);

	//pcl::PointCloud<PointT> Final;
	reg.align(*src_corr);

	//*res = *src_corr + *tgt_corr;

	cout << "there are " << res->points.size() << " in res " << std::endl;
	std::cout << "conv ? " << reg.hasConverged() << ", score ? " << reg.getFitnessScore() << std::endl;

	//S(*transformation) = reg.getFinalTransformation();
	std::cout << reg.getFinalTransformation() << std::endl;
	// Run the same optimization in a loop and visualize the results
	//Eigen::Matrix4f Ti = Eigen::Matrix4f::Identity(), prev;

	//reg.setMaximumIterations(1000);

	//// Get the transformation from target to source
	//Ti = reg.getFinalTransformation();
	//(*transformation) = Ti.inverse();
	//// Print matrix
	//for (int i = 0; i < 4; i++) {
	//	for (int j = 0; j < 4; j++)
	//		cout << (*transformation)(i, j) << " | ";
	//	cout << endl;
	//}
}

// COLORED VERSION
void showCorrespondences(PointCloudColored::Ptr src, PointCloudColored::Ptr tgt,
	PointCloudColored::Ptr src_corr, PointCloudColored::Ptr tgt_corr,
	pcl::CorrespondencesPtr correspondences, float distance_max) {
/*
	MetricVisualizer v(strdup("SOME FRAME"), src);*/
	pcl::visualization::PCLVisualizer::Ptr v(new pcl::visualization::PCLVisualizer);
	int v1(10), v2(11);
	v->setShowFPS(true);
	v->createViewPort(0.0, 0.0, 1.0, 0.5, v1);
	v->createViewPort(0.0, 0.5, 1.0, 1.0, v2);

	v->registerKeyboardCallback(keyboardEventOccurred, (void*)&v);
	v->registerMouseCallback(mouseEventOccurred, (void*)&v);
	struct callback_args cb_args;
	PointCloud::Ptr clicked_points_3d(new PointCloud);
	cb_args.clicked_points_3d = clicked_points_3d;
	cb_args.viewerPtr = Visualizer::Ptr(v);
	cb_args.distance = new char[128];
	sprintf(cb_args.distance, "%f", 0);
	v->registerPointPickingCallback(pp_callback, (void*)&cb_args);

	cout << "Correspondences found: " << correspondences->size() << endl;
	for (int i = 0; i < correspondences->size(); i++) {
		if (correspondences->at(i).index_match != -1) { // only add if correspondence found
			src_corr->push_back(src->at(correspondences->at(i).index_query));
			tgt_corr->push_back(tgt->at(correspondences->at(i).index_match));
			distance_max = std::fmaxf(correspondences->at(i).distance, distance_max);
		}
	}
	cout << src_corr->size() << " Correpondences rly found" << endl;
	// Content
	PointCloudColorHandlerCustom<PointColorT> src_rgb(src, 210, 50, 50); // Red
	PointCloudColorHandlerCustom<PointColorT> tgt_rgb(tgt, 50, 210, 50); // Green
	PointCloudColorHandlerCustom<PointColorT> src_corr_rgb(src, 50, 210, 210); // Red contrary
	PointCloudColorHandlerCustom<PointColorT> tgt_corr_rgb(tgt, 210, 50, 210); // Green contrary
	v->addPointCloud(src, src_rgb, "v1_source", v1);
	v->addPointCloud(tgt, tgt_rgb, "v1_target", v1);
	v->addPointCloud(src_corr, src_corr_rgb, "v2_source", v2);
	v->addPointCloud(tgt_corr, tgt_corr_rgb, "v2_target", v2);

	cout << "max distance: " << distance_max << endl;
	// Spinning (drawing result)
	v->spin();
}



// NO COLOR VERSION
void showCorrespondences(PointCloud::Ptr src, PointCloud::Ptr tgt,
	PointCloud::Ptr src_corr, PointCloud::Ptr tgt_corr,
	pcl::CorrespondencesPtr correspondences, float distance_max) {

	MetricVisualizer * v = new MetricVisualizer (strdup("SOME FRAME"), src);
	//pcl::visualization::PCLVisualizer::Ptr v(new pcl::visualization::PCLVisualizer);
	int v1(10), v2(11);
	//v->setShowFPS(true);
	v->createViewPort(0.0, 0.0, 1.0, 0.5, v1);
	v->createViewPort(0.0, 0.5, 1.0, 1.0, v2);

	//v->registerKeyboardCallback(keyboardEventOccurred, (void*)&v);
	////v->registerMouseCallback(mouseEventOccurred, (void*)&v);
	//struct callback_args cb_args;
	//PointCloud::Ptr clicked_points_3d(new PointCloud);
	//cb_args.clicked_points_3d = clicked_points_3d;
	//cb_args.viewerPtr = pcl::visualization::PCLVisualizer::Ptr(v);
	//cb_args.distance = new char[128];
	//sprintf(cb_args.distance, "%f", 0);
	//v->registerPointPickingCallback(pp_callback, (void*)&cb_args);

	cout << "Correspondences found: " << correspondences->size() << endl;
	for (int i = 0; i < correspondences->size(); i++) {
		if (correspondences->at(i).index_match != -1) { // only add if correspondence found
			src_corr->push_back(src->at(correspondences->at(i).index_query));
			tgt_corr->push_back(tgt->at(correspondences->at(i).index_match));
			distance_max = std::fmaxf(correspondences->at(i).distance, distance_max);
		}
	}
	cout << src_corr->size() << " Correpondences rly found" << endl;
	// Content
	PointCloudColorHandlerCustom<PointT> src_rgb(src, 210, 50, 50); // Red
	PointCloudColorHandlerCustom<PointT> tgt_rgb(tgt, 50, 210, 50); // Green
	PointCloudColorHandlerCustom<PointT> src_corr_rgb(src, 50, 210, 210); // Red contrary
	PointCloudColorHandlerCustom<PointT> tgt_corr_rgb(tgt, 210, 50, 210); // Green contrary
	v->addPointCloud(src, src_rgb, "v1_source", v1);
	v->addPointCloud(tgt, tgt_rgb, "v1_target", v1);
	v->addPointCloud(src_corr, src_corr_rgb, "v2_source", v2);
	v->addPointCloud(tgt_corr, tgt_corr_rgb, "v2_target", v2);

	cout << "max distance: " << distance_max << endl;
	// Spinning (drawing result)
	v->spin();
}

char * TOTO1() {
	return strdup("PourJulien1.ply");
}

char * TOTO2() {
	return strdup("PourJulien2.ply");
}

#include <pcl/registration/lum.h>
#include <pcl/registration/transformation_estimation_point_to_plane_lls.h>
void method_nico(PointCloud::Ptr source, PointCloud::Ptr target, PointCloud::Ptr result) {

	PointCloud::Ptr src_keypoints(new PointCloud);
	PointCloud::Ptr tgt_keypoints(new PointCloud);

	IndicesT::Ptr src_indices(new IndicesT);
	IndicesT::Ptr tgt_indices(new IndicesT);

	if (boost::filesystem::exists("keypoints1.ply")) IOPLY::load(strdup("keypoints1.ply"), src_keypoints);
	else { ISS(source, src_keypoints, src_indices); IOPLY::save(strdup("keypoints1.ply"), src_keypoints); }
	if (boost::filesystem::exists("keypoints2.ply")) IOPLY::load(strdup("keypoints2.ply"), tgt_keypoints);
	else { ISS(target, tgt_keypoints, tgt_indices); IOPLY::save(strdup("keypoints2.ply"), tgt_keypoints); }

	/*ISS(source, src_keypoints, src_indices);
	ISS(target, tgt_keypoints, tgt_indices);*/

	// Use your favorite pairwise correspondence estimation algorithm(s)
	pcl::CorrespondencesPtr corrs_0_to_1(new pcl::Correspondences); //= someAlgo(source, target);
	pcl::CorrespondencesPtr corrs_1_to_0(new pcl::Correspondences); // = someAlgo(target, source);
	pcl::registration::CorrespondenceEstimation<PointT, PointT> cest;
	// Correspondences parameters
	cest.setInputSource(src_keypoints);
	cest.setInputTarget(tgt_keypoints);
	// Correspondences result
	cest.determineCorrespondences(*corrs_0_to_1);
	// Correspondences parameters
	cest.setInputSource(tgt_keypoints);
	cest.setInputTarget(src_keypoints);
	// Correspondences result
	cest.determineCorrespondences(*corrs_1_to_0);
	
	
	//Eigen::Matrix4f transformation;
	//pcl::registration::TransformationEstimationPointToPlaneLLS<boost::shared_ptr<pcl::PointCloud<pcl::PointXYZ>>,
	//	boost::shared_ptr<pcl::PointCloud<pcl::PointXYZ>>, Eigen::Matrix4>	teptplls;
	//teptplls.estimateRigidTransformation(source, target, &transformation);

	//pcl::registration::LUM<PointT> lum;
	//// Add point clouds as vertices to the SLAM graph
	//lum.addPointCloud(source);
	//lum.addPointCloud(target);
	//// Add the correspondence results as edges to the SLAM graph
	//lum.setCorrespondences(0, 1, corrs_0_to_1);
	//lum.setCorrespondences(1, 0, corrs_1_to_0);
	//// Change the computation parameters
	//lum.setMaxIterations(150);
	//lum.setConvergenceThreshold(0.0);
	//// Perform the actual LUM computation
	//lum.compute();
	//// Return the concatenated point cloud result
	//result = lum.getConcatenatedCloud();
	// Return the separate point cloud transformations
	/*for (int i = 0; i < lum.getNumVertices(); i++)
	{
		transforms_out[i] = lum.getTransformation(i);
	}*/


	Visualizer::Ptr v(new Visualizer);
	v->setShowFPS(true);
	v->addPointCloud(result);
	v->spin();

}

Status ProcessorPCL::merge(string path) {
	Status s;
	char* f1 = TOTO1();
	char* f2 = TOTO2();

	// ----- Loading ply files -----
	PointCloud::Ptr src_raw(new PointCloud);
	PointCloud::Ptr tgt_raw(new PointCloud);

	// ----- Sampling clouds -----
	PointCloud::Ptr src(new PointCloud);
	PointCloud::Ptr tgt(new PointCloud);

	if (boost::filesystem::exists("organized1.ply")) IOPLY::load(strdup("organized1.ply"), src);
	else { IOPLY::load(f1, src_raw); reduce_size(src_raw, src); IOPLY::save(strdup("organized1.ply"), src); }
	if (boost::filesystem::exists("organized2.ply")) IOPLY::load(strdup("organized2.ply"), tgt);
	else { IOPLY::load(f2, tgt_raw); reduce_size(tgt_raw, tgt); IOPLY::save(strdup("organized2.ply"), tgt); }

	PointCloud::Ptr res(new PointCloud);
	method_nico(src, tgt, res);
	//// ----- Keypoints ------
	//PointCloud::Ptr src_keypoints(new PointCloud);
	//PointCloud::Ptr tgt_keypoints(new PointCloud);
	//IndicesT::Ptr src_indices(new IndicesT);
	//IndicesT::Ptr tgt_indices(new IndicesT);

	//if (boost::filesystem::exists("keypoints1.ply")) IOPLY::load(strdup("keypoints1.ply"), src_keypoints);
	//else { ISS(src, src_keypoints, src_indices); IOPLY::save(strdup("keypoints1.ply"), src_keypoints); }
	//if (boost::filesystem::exists("keypoints2.ply")) IOPLY::load(strdup("keypoints2.ply"), tgt_keypoints);
	//else { ISS(tgt, tgt_keypoints, tgt_indices); IOPLY::save(strdup("keypoints2.ply"), tgt_keypoints); }

	//ISS(src, src_keypoints, src_indices);
	//ISS(tgt, tgt_keypoints, tgt_indices);

	//// ----- Normals -----
	//PointCloudNormals::Ptr src_normals_raw(new PointCloudNormals);
	//PointCloudNormals::Ptr tgt_normals_raw(new PointCloudNormals);
	//PointCloudNormals::Ptr src_normals(new PointCloudNormals);
	//PointCloudNormals::Ptr tgt_normals(new PointCloudNormals);
	///*	if (boost::filesystem::exists("normals1.pcd")) LoadNormals("normals1.pcd", src_normals);
	//else { normals(src_keypoints, src_normals); SaveNormals("normals1.pcd", src_keypoints, src_normals); }
	//if (boost::filesystem::exists("normals2.pcd")) LoadNormals("normals2.pcd", tgt_normals);
	//else { normals(tgt_keypoints, tgt_normals); SaveNormals("normals2.pcd", tgt_keypoints, tgt_normals); } */
	///*	normals(src, src_normals_raw);
	//normals(tgt, tgt_normals_raw);

	//for (int i = 0; i < src_indices->indices.size(); i++)
	//src_normals->push_back(
	//src_normals_raw->at(
	//src_indices->indices.at(i)));
	//for (int i = 0; i < tgt_indices->indices.size(); i++)
	//tgt_normals->push_back(
	//tgt_normals_raw->at(
	//tgt_indices->indices.at(i))); */

	//// ----- Features ------
	///* PointCloudFPFH::Ptr src_features(new PointCloudFPFH);
	//PointCloudFPFH::Ptr tgt_features(new PointCloudFPFH);
	//if (boost::filesystem::exists("features1.ply")) LoadFeatures("features1.ply", src_features);
	//else { FPFH(src, src_normals, src_keypoints, src_features); SaveFeatures("features1.ply", src_features); }
	//if (boost::filesystem::exists("features2.ply")) LoadFeatures("features2.ply", tgt_features);
	//else { FPFH(tgt, tgt_normals, tgt_keypoints, tgt_features); SaveFeatures("features2.ply", tgt_features); } */

	//// ----- Correspondences -----
	//pcl::CorrespondencesPtr correspondences(new pcl::Correspondences);
	//pcl::registration::CorrespondenceEstimation<PointT, PointT> cest;
	//PointCloud::Ptr src_corr(new PointCloud), tgt_corr(new PointCloud);
	//// Correspondences parameters
	//cest.setInputSource(src_keypoints);
	//cest.setInputTarget(tgt_keypoints);
	//// Correspondences result
	//cest.determineCorrespondences(*correspondences);
	//float distance_max = 0;
	//showCorrespondences(src_keypoints, tgt_keypoints, src_corr, tgt_corr, correspondences, distance_max);

	//// ----- Registration ------
	//Eigen::Matrix4f transformation;
	//// Estimated transformation
	//// registration_transformation_estimation(src_features, tgt_features, src_keypoints, tgt_keypoints, &transformation);
	//PointCloud::Ptr res(new PointCloud);
	//registration_ICP(src_corr, tgt_corr, res, distance_max, &transformation);
	//Eigen::Matrix4f inverse = transformation.inverse();

	///* PointCloud::Ptr res_xyz(new PointCloud);
	//for (int i = 0; i < res->size(); i++)
	//res_xyz->push_back(PointT(res->at(i).x, res->at(i).y, res->at(i).z)); */

	//pcl::transformPointCloud(*tgt, *res, transformation);
	//pcl::transformPointCloud(*tgt, *tgt, inverse);
	//// ----- Show clouds -----
	//showClouds(src, tgt, res, src_keypoints, src_normals, tgt_keypoints, tgt_normals);
	return s;
}

Status ProcessorPCL::computeMetrics() {
	Status s;
	return s;
}

Status ProcessorPCL::computeMetrics(float x1, float y1, float z1, float x2, float y2, float z2) {
	Status s;
	return s;
}

