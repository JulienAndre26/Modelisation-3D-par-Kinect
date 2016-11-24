#include "stdafx.h"

#pragma warning( disable : 4996 )
/* #include <cstdio>
#include <stdio.h>
#include <iostream>
#include <string>

#include <pcl/kdtree/kdtree_flann.h>
#include <pcl/features/normal_3d.h>
#include <pcl/surface/gp3.h>

#include <pcl/io/ply_io.h>
#include <pcl/visualization/pcl_visualizer.h>

using namespace pcl;
using namespace std;


#ifdef WIN32
# define sleep(x) Sleep((x)*1000)
#endif

int
main(int argc, char **argv)
{
	string bleh = "PourJulienColor1.ply";
	
	pcl::PointCloud<pcl::PointXYZRGB>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZRGB>);
	pcl::io::loadPLYFile<pcl::PointXYZRGB>(bleh, *cloud);

	cout << cloud->size() << endl;
	cout << "[DEBUG] LOADED" << endl;

	pcl::visualization::PCLVisualizer* viewer(new pcl::visualization::PCLVisualizer);

	pcl::NormalEstimation<pcl::PointXYZRGB, pcl::Normal> n;
	pcl::PointCloud<pcl::Normal>::Ptr normals(new pcl::PointCloud<pcl::Normal>);
	pcl::search::KdTree<pcl::PointXYZRGB>::Ptr tree(new pcl::search::KdTree<pcl::PointXYZRGB>);
	tree->setInputCloud(cloud);
	n.setInputCloud(cloud);
	n.setSearchMethod(tree);
	n.setKSearch(20);
	n.compute(*normals);
	//* normals should not contain the point normals + surface curvatures
	cout << "[DEBUG] NORMALIZED" << endl;

	// Concatenate the XYZ and normal fields*
	pcl::PointCloud<pcl::PointXYZRGBNormal>::Ptr cloud_with_normals(new pcl::PointCloud<pcl::PointXYZRGBNormal>);
	pcl::concatenateFields(*cloud, *normals, *cloud_with_normals);
	//* cloud_with_normals = cloud + normals
	cout << "[DEBUG] CONCANETED" << endl;

	// Create search tree*
	pcl::search::KdTree<pcl::PointXYZRGBNormal>::Ptr tree2(new pcl::search::KdTree<pcl::PointXYZRGBNormal>);
	tree2->setInputCloud(cloud_with_normals);

	// Initialize objects
	pcl::GreedyProjectionTriangulation<pcl::PointXYZRGBNormal> gp3;
	pcl::PolygonMesh triangles;

	// Set the maximum distance between connected points (maximum edge length)
	gp3.setSearchRadius(0.025);

	// Set typical values for the parameters
	gp3.setMu(2.5);
	gp3.setMaximumNearestNeighbors(100);
	gp3.setMaximumSurfaceAngle(M_PI / 4); // 45 degrees
	gp3.setMinimumAngle(M_PI / 18); // 10 degrees
	gp3.setMaximumAngle(2 * M_PI / 3); // 120 degrees
	gp3.setNormalConsistency(false);

	// Get result
	gp3.setInputCloud(cloud_with_normals);
	gp3.setSearchMethod(tree2);
	gp3.reconstruct(triangles);

	cout << "[DEBUG] TRIANGULARIZED" << endl;

	// Additional vertex information
	std::vector<int> parts = gp3.getPartIDs();
	std::vector<int> states = gp3.getPointStates();

	viewer->setShapeRenderingProperties(pcl::visualization::PCL_VISUALIZER_SHADING,
		pcl::visualization::PCL_VISUALIZER_SHADING_PHONG, "polygon");
	viewer->addPolygonMesh(triangles);

	cout << "[DEBUG] RENDERING..." << endl;

	// show decompressed point cloud
	while (!viewer->wasStopped())
		viewer->spinOnce(100); 

	return EXIT_SUCCESS;
	*/
	/*	int v1(0);
	viewer->createViewPort(0.0, 0.0, 1.0, 1.0, v1);
	viewer->setBackgroundColor(0.2, 0.2, 0.2, v1);
	// pcl::visualization::PointCloudColorHandlerCustom<pcl::PointXYZRGB> rgb(cloud, 230, 40, 120);
	pcl::visualization::PointCloudColorHandlerRGBField<pcl::PointXYZRGB> rgb(cloud);
	viewer->addPointCloud<pcl::PointXYZRGB>(cloud, rgb, "Before Cloud", v1); */

	/*	int v2(0);
	viewer->createViewPort(0.5, 0.0, 1.0, 1.0, v2);
	viewer->setBackgroundColor(0.2, 0.2, 0.2, v2);
	pcl::visualization::PointCloudColorHandlerRGBField<pcl::PointXYZRGB> single_color(cloudOut);
	viewer->addPointCloud<pcl::PointXYZRGB>(cloudOut, single_color, "AfterCloud", v2);
	*/
/* } */

/*
* Software License Agreement (BSD License)
*
*  Copyright (c) 2010, Willow Garage, Inc.
*  All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without
*  modification, are permitted provided that the following conditions
*  are met:
*
*   * Redistributions of source code must retain the above copyright
*     notice, this list of conditions and the following disclaimer.
*   * Redistributions in binary form must reproduce the above
*     copyright notice, this list of conditions and the following
*     disclaimer in the documentation and/or other materials provided
*     with the distribution.
*   * Neither the name of Willow Garage, Inc. nor the names of its
*     contributors may be used to endorse or promote products derived
*     from this software without specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
*  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
*  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
*  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
*  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
*  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
*  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
*  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
*  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
*  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
*  POSSIBILITY OF SUCH DAMAGE.
*
* $Id$
*
*/

//* \author Radu Bogdan Rusu
//* adaptation Raphael Favier

#include <boost/make_shared.hpp>
#include <pcl/point_types.h>
#include <pcl/point_cloud.h>
#include <pcl/point_representation.h>

#include <pcl/io/ply_io.h>

#include <pcl/filters/voxel_grid.h>
#include <pcl/filters/filter.h>

#include <pcl/features/normal_3d.h>

#include <pcl/registration/icp.h>
#include <pcl/registration/icp_nl.h>
#include <pcl/registration/transforms.h>

#include <pcl/visualization/pcl_visualizer.h>

using pcl::visualization::PointCloudColorHandlerGenericField;
using pcl::visualization::PointCloudColorHandlerCustom;

//convenient typedefs
typedef pcl::PointXYZ PointT;
typedef pcl::PointCloud<PointT> PointCloud;
typedef pcl::PointNormal PointNormalT;
typedef pcl::PointCloud<PointNormalT> PointCloudWithNormals;
typedef pcl::FPFHSignature33 FPFHSignature33T;
typedef pcl::PointCloud<FPFHSignature33T> PointCloudFPFH;

// This is a tutorial so we can afford having global variables 
//our visualizer
pcl::visualization::PCLVisualizer *p;
//its left and right viewports
int vp_1, vp_2;

//convenient structure to handle our pointclouds
struct PLY {
	PointCloud::Ptr cloud;
	std::string f_name;

	PLY() : cloud(new PointCloud) {};
};

struct PLYComparator {
	bool operator () (const PLY& p1, const PLY& p2) {
		return (p1.f_name < p2.f_name);
	}
};


// Define a new point representation for < x, y, z, curvature >
class MyPointRepresentation : public pcl::PointRepresentation <PointNormalT>
{
	using pcl::PointRepresentation<PointNormalT>::nr_dimensions_;
public:
	MyPointRepresentation()
	{
		// Define the number of dimensions
		nr_dimensions_ = 4;
	}

	// Override the copyToFloatArray method to define our feature vector
	virtual void copyToFloatArray(const PointNormalT &p, float * out) const
	{
		// < x, y, z, curvature >
		out[0] = p.x;
		out[1] = p.y;
		out[2] = p.z;
		out[3] = p.curvature;
	}
};


////////////////////////////////////////////////////////////////////////////////
//** \brief Display source and target on the first viewport of the visualizer
void showCloudsLeft(const PointCloud::Ptr cloud_target, const PointCloud::Ptr cloud_source)
{
	p->removePointCloud("vp1_target");
	p->removePointCloud("vp1_source");

	PointCloudColorHandlerCustom<PointT> tgt_h(cloud_target, 0, 255, 0);
	PointCloudColorHandlerCustom<PointT> src_h(cloud_source, 255, 0, 0);
	p->addPointCloud(cloud_target, tgt_h, "vp1_target", vp_1);
	p->addPointCloud(cloud_source, src_h, "vp1_source", vp_1);

	PCL_INFO("Press q to begin the registration.\n");
	p->spin();
}


////////////////////////////////////////////////////////////////////////////////
//** \brief Display source and target on the second viewport of the visualizer
void showCloudsRight(const PointCloudWithNormals::Ptr cloud_target, const PointCloudWithNormals::Ptr cloud_source)
{
	p->removePointCloud("source");
	p->removePointCloud("target");


	PointCloudColorHandlerGenericField<PointNormalT> tgt_color_handler(cloud_target, "curvature");
	if (!tgt_color_handler.isCapable())
		PCL_WARN("Cannot create curvature color handler!");

	PointCloudColorHandlerGenericField<PointNormalT> src_color_handler(cloud_source, "curvature");
	if (!src_color_handler.isCapable())
		PCL_WARN("Cannot create curvature color handler!");


	p->addPointCloud(cloud_target, tgt_color_handler, "target", vp_2);
	p->addPointCloud(cloud_source, src_color_handler, "source", vp_2);

	p->spinOnce();
}

////////////////////////////////////////////////////////////////////////////////
//** \brief Load a set of PLY files that we want to register together
//* \param argc the number of arguments (pass from main ())
//* \param argv the actual command line arguments (pass from main ())
//* \param models the resultant vector of point cloud datasets
void loadData(int argc, char **argv, std::vector<PLY, Eigen::aligned_allocator<PLY> > &models)
{
	std::string extension(".ply");
	// Suppose the first argument is the actual test model
	for (int i = 1; i < argc; i++)
	{
		std::string fname = std::string(argv[i]);
		// Needs to be at least 5: .plot
		if (fname.size() <= extension.size())
			continue;

		std::transform(fname.begin(), fname.end(), fname.begin(), (int(*)(int))tolower);

		//check that the argument is a ply file
		if (fname.compare(fname.size() - extension.size(), extension.size(), extension) == 0)
		{
			// Load the cloud and saves it into the global list of models
			PLY m;
			m.f_name = argv[i];
			pcl::io::loadPLYFile(argv[i], *m.cloud);
			//remove NAN points from the cloud
			std::vector<int> indices;
			pcl::removeNaNFromPointCloud(*m.cloud, *m.cloud, indices);

			models.push_back(m);
		}
	}
}


////////////////////////////////////////////////////////////////////////////////
//** \brief Align a pair of PointCloud datasets and return the result
//* \param cloud_src the source PointCloud
//* \param cloud_tgt the target PointCloud
//* \param output the resultant aligned source PointCloud
//* \param final_transform the resultant transform between source and target
void pairAlign(const PointCloud::Ptr cloud_src, const PointCloud::Ptr cloud_tgt, PointCloud::Ptr output, Eigen::Matrix4f &final_transform, bool downsample = false)
{
	// Downsample for consistency and speed
	// \note enable this for large datasets
	PointCloud::Ptr src(new PointCloud);
	PointCloud::Ptr tgt(new PointCloud);
	pcl::VoxelGrid<PointT> grid;
	if (downsample)
	{
		grid.setLeafSize(0.05, 0.05, 0.05);
		grid.setInputCloud(cloud_src);
		grid.filter(*src);

		grid.setInputCloud(cloud_tgt);
		grid.filter(*tgt);
	}
	else
	{
		src = cloud_src;
		tgt = cloud_tgt;
	}


	// Compute surface normals and curvature
	PointCloudWithNormals::Ptr points_with_normals_src(new PointCloudWithNormals);
	PointCloudWithNormals::Ptr points_with_normals_tgt(new PointCloudWithNormals);

	pcl::NormalEstimation<PointT, PointNormalT> norm_est;
	pcl::search::KdTree<pcl::PointXYZ>::Ptr tree(new pcl::search::KdTree<pcl::PointXYZ>());
	norm_est.setSearchMethod(tree);
	norm_est.setKSearch(30);

	norm_est.setInputCloud(src);
	norm_est.compute(*points_with_normals_src);
	pcl::copyPointCloud(*src, *points_with_normals_src);

	norm_est.setInputCloud(tgt);
	norm_est.compute(*points_with_normals_tgt);
	pcl::copyPointCloud(*tgt, *points_with_normals_tgt);

	// Instantiate our custom point representation (defined above) ...
	MyPointRepresentation point_representation;
	// ... and weight the 'curvature' dimension so that it is balanced against x, y, and z
	float alpha[4] = { 1.0, 1.0, 1.0, 1.0 };
	point_representation.setRescaleValues(alpha);

	// Align
	pcl::IterativeClosestPointNonLinear<PointNormalT, PointNormalT> reg;
	reg.setTransformationEpsilon(1e-6);
	// Set the maximum distance between two correspondences (src<->tgt) to 10cm
	// Note: adjust this based on the size of your datasets
	reg.setMaxCorrespondenceDistance(0.1);
	// Set the point representation
	reg.setPointRepresentation(boost::make_shared<const MyPointRepresentation>(point_representation));

	reg.setInputSource(points_with_normals_src);
	reg.setInputTarget(points_with_normals_tgt);

	// Run the same optimization in a loop and visualize the results
	Eigen::Matrix4f Ti = Eigen::Matrix4f::Identity(), prev, targetToSource;
	PointCloudWithNormals::Ptr reg_result = points_with_normals_src;
	reg.setMaximumIterations(10);
	for (int i = 0; i < 30; ++i)
	{
		PCL_INFO("Iteration Nr. %d.\n", i);

		// save cloud for visualization purpose
		points_with_normals_src = reg_result;

		// Estimate
		reg.setInputSource(points_with_normals_src);
		reg.align(*reg_result);

		//accumulate transformation between each Iteration
		Ti = reg.getFinalTransformation() * Ti;

		//if the difference between this transformation and the previous one
		//is smaller than the threshold, refine the process by reducing
		//the maximal correspondence distance
		if (fabs((reg.getLastIncrementalTransformation() - prev).sum()) < reg.getTransformationEpsilon())
			reg.setMaxCorrespondenceDistance(reg.getMaxCorrespondenceDistance() - 0.001);

		prev = reg.getLastIncrementalTransformation();

		// visualize current state
		showCloudsRight(points_with_normals_tgt, points_with_normals_src);
	}

	// Get the transformation from target to source
	targetToSource = Ti.inverse();

	// Transform target back in source frame
	pcl::transformPointCloud(*cloud_tgt, *output, targetToSource);

	p->removePointCloud("source");
	p->removePointCloud("target");

	PointCloudColorHandlerCustom<PointT> cloud_tgt_h(output, 0, 255, 0);
	PointCloudColorHandlerCustom<PointT> cloud_src_h(cloud_src, 255, 0, 0);
	p->addPointCloud(output, cloud_tgt_h, "target", vp_2);
	p->addPointCloud(cloud_src, cloud_src_h, "source", vp_2);

	PCL_INFO("Press q to continue the registration.\n");
	p->spin();

	p->removePointCloud("source");
	p->removePointCloud("target");

	//add the source to the transformed target
	*output += *cloud_src;

	final_transform = targetToSource;
}

/*
int ICP() {
	PCL_INFO("ICP METHOD");
	// Command line simulation
	int argc = 3;
	char** argv;
	argv[1] = "PourJulienColor1.ply";
	argv[2] = "PourJulienColor2.ply";

	// Load data
	std::vector<PLY, Eigen::aligned_allocator<PLY> > data;
	loadData(argc, argv, data);

	// Check user input
	if (data.empty())
	{
		PCL_ERROR("Syntax is: %s <source.ply> <target.ply> [*]", argv[0]);
		PCL_ERROR("[*] - multiple files can be added. The registration results of (i, i+1) will be registered against (i+2), etc");
		return (-1);
	}
	PCL_INFO("Loaded %d datasets.", (int)data.size());

	// Create a PCLVisualizer object
	p = new pcl::visualization::PCLVisualizer(argc, argv, "Pairwise Incremental Registration example");
	p->createViewPort(0.0, 0, 0.5, 1.0, vp_1);
	p->createViewPort(0.5, 0, 1.0, 1.0, vp_2);

	PointCloud::Ptr result(new PointCloud), source, target;
	Eigen::Matrix4f GlobalTransform = Eigen::Matrix4f::Identity(), pairTransform;

	for (size_t i = 1; i < data.size(); ++i)
	{
		source = data[i - 1].cloud;
		target = data[i].cloud;

		cout << "bleh before cloud left" << endl;

		// Add visualization data
		showCloudsLeft(source, target);

		cout << "bleh after cloud left" << endl;

		PointCloud::Ptr temp(new PointCloud);
		PCL_INFO("Aligning %s (%d) with %s (%d).\n", data[i - 1].f_name.c_str(), source->points.size(), data[i].f_name.c_str(), target->points.size());
		pairAlign(source, target, temp, pairTransform, true);

		//transform current pair into the global transform
		pcl::transformPointCloud(*temp, *result, GlobalTransform);

		//update the global transform
		GlobalTransform = GlobalTransform * pairTransform;

		//save aligned pair, transformed into the first cloud's frame
		std::stringstream ss;
		ss << i << ".ply";
		pcl::io::savePLYFile(ss.str(), *result, true);
	}
	return EXIT_SUCCESS;
}
*/
// ------------------------------------------------------------------------------------------------//

#include <pcl/filters/voxel_grid.h>
void reduce_size(PointCloud::Ptr cloud, PointCloud::Ptr res) {
	std::cerr << "PointCloud before filtering: " << cloud->width * cloud->height
		<< " data points (" << pcl::getFieldsList(*cloud) << ")." << endl;

	// Create the filtering object
	pcl::VoxelGrid<PointT> sor;
	sor.setInputCloud(cloud);
	sor.setLeafSize(0.01f, 0.01f, 0.01f);
	sor.filter(*res);

	std::cerr << "PointCloud after filtering: " << res->width * res->height
		<< " data points (" << pcl::getFieldsList(*res) << ")." << endl;

	// Write filtered object
/*	pcl::PLYWriter writer;
	writer.write("table_scene_lms400_downsampled.pcd", *cloud_filtered,
		Eigen::Vector4f::Zero(), Eigen::Quaternionf::Identity(), false); */
}

double computeCloudResolution(const PointCloud::ConstPtr &cloud)
{
	double res = 0.0;
	int n_points = 0;
	int nres;
	std::vector<int> indices(2);
	std::vector<float> sqr_distances(2);
	pcl::search::KdTree<PointT> tree;
	tree.setInputCloud(cloud);

	for (size_t i = 0; i < cloud->size(); ++i)
	{
		if (!pcl_isfinite((*cloud)[i].x))
		{
			continue;
		}
		//Considering the second neighbor since the first is the point itself.
		nres = tree.nearestKSearch(i, 2, indices, sqr_distances);
		if (nres == 2)
		{
			res += sqrt(sqr_distances[1]);
			++n_points;
		}
	}
	if (n_points != 0)
	{
		res /= n_points;
	}
	return res;
}

#include <pcl/keypoints/iss_3d.h>

void ISS(PointCloud::Ptr cloud, PointCloud::Ptr keypoints) {
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
	cout << "Keypoints found : " << keypoints->size() << endl;
}

#include <pcl/features/fpfh.h>
void FPFH(PointCloud::Ptr cloud, PointCloudWithNormals::Ptr normals, PointCloud::Ptr keypoints, PointCloudFPFH::Ptr features) {
	cout << "Cloud with size = " << cloud->size() << endl;
	// ----- FPFH features -----
	pcl::FPFHEstimation<PointT, PointNormalT, FPFHSignature33T> fpfh;
	// FPFH parameters
	float radius(0.002); 
	fpfh.setSearchSurface(cloud);
	fpfh.setInputNormals(normals);
	fpfh.setRadiusSearch(radius);
	fpfh.setInputCloud(keypoints);
	// FPFH result
	fpfh.compute(*features);
	cout << "Features found : " << features->size() << endl;
}

#include <pcl/registration/correspondence_estimation.h>
#include <pcl/registration/correspondence_rejection_sample_consensus.h>
#include <pcl/registration/transformation_estimation_svd.h>
void registration(PointCloudFPFH::Ptr source_features, PointCloudFPFH::Ptr target_features, 
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
	rejector.setInlierThreshold(2.5);
	rejector.setMaximumIterations(1000000);
	rejector.setRefineModel(false);
	rejector.setInputCorrespondences(correspondences);
	// Rejector result
	rejector.getCorrespondences(*corr_filtered);
	
	// ----- Transformation estimation -----
	pcl::registration::TransformationEstimationSVD<PointT, PointT> trans_est;
	trans_est.estimateRigidTransformation(*source_keypoints,*target_keypoints,*corr_filtered, *transform);
}

#include <pcl/features/normal_3d.h>
void normals(PointCloud::Ptr cloud, PointCloudWithNormals::Ptr normals) {
	// ----- Normal estimation -----
	pcl::NormalEstimation<PointT, PointNormalT> ne;
	// Normal parameters
	ne.setInputCloud(cloud);
	// Tree search
	pcl::search::KdTree<PointT>::Ptr tree(new pcl::search::KdTree<PointT>());
	ne.setSearchMethod(tree);
	ne.setRadiusSearch(0.03);
	// Normal result
	ne.compute(*normals);
}

#include <pcl/visualization/pcl_visualizer.h>
void showClouds(PointCloud::Ptr src, PointCloud::Ptr tgt) {
	pcl::visualization::PCLVisualizer::Ptr v(new pcl::visualization::PCLVisualizer);

	int v1(1), v2(2);
	v->createViewPort(0.0, 0.0, 0.5, 1.0, v1); // left view
	// Left view content
	PointCloudColorHandlerCustom<PointT> src1_rgb(src, 220, 30, 30);
	PointCloudColorHandlerCustom<PointT> src2_rgb(tgt, 30, 220, 30);
	v->addPointCloud(src, src1_rgb, "v1_source", v1);
	v->addPointCloud(tgt, src2_rgb, "v1_target", v1);
	
	v->createViewPort(0.5, 0.0, 1.0, 1.0, v2); // right view
	// Right view content
	PointCloudColorHandlerCustom<PointT> tgt_rgb(tgt, 190, 190, 190); // snowy white
	v->addPointCloud(tgt, tgt_rgb, "v2_source", v2);
	v->spin();
}

int main(int argc, char** argv)
{
	argv[1] = "PourJulien1.ply";
	argv[2] = "PourJulien2.ply";

	// ----- Loading ply files -----
	PointCloud::Ptr src_raw(new PointCloud);
	PointCloud::Ptr tgt_raw(new PointCloud);
	pcl::io::loadPLYFile(argv[1],*src_raw);
	pcl::io::loadPLYFile(argv[2],*tgt_raw);

	// ----- Sampling clouds -----
	PointCloud::Ptr src(new PointCloud);
	PointCloud::Ptr tgt(new PointCloud);
	reduce_size(src_raw, src);
	reduce_size(tgt_raw, tgt);

	// ----- Normals -----
	PointCloudWithNormals::Ptr src_normals(new PointCloudWithNormals);
	PointCloudWithNormals::Ptr tgt_normals(new PointCloudWithNormals);
	normals(src, src_normals);
	normals(tgt, tgt_normals);

	// ----- Keypoints ------
	PointCloud::Ptr src_keypoints(new PointCloud);
	PointCloud::Ptr tgt_keypoints(new PointCloud);
	ISS(src, src_keypoints);
	ISS(tgt, tgt_keypoints);

	// ----- Features ------
	PointCloudFPFH::Ptr src_features(new PointCloudFPFH);
	PointCloudFPFH::Ptr tgt_features(new PointCloudFPFH);
	FPFH(src, src_normals, src_keypoints, src_features);
	FPFH(tgt, tgt_normals, tgt_keypoints, tgt_features);

	// ----- Registration ------
	Eigen::Matrix4f transformation;
	// Estimated transformation
	registration(src_features, tgt_features, src_keypoints, tgt_keypoints, &transformation);
	Eigen::Matrix4f inverse = transformation.inverse();

	cout << "Inverse size : " << inverse.size() << endl;

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++)
			cout << inverse(i,j) << " | ";
		cout << endl;
	}

	// Apply transformation to clouds
	PointCloud::Ptr res(new PointCloud);
	pcl::transformPointCloud(*tgt, *res, inverse);

	// ----- Show clouds -----
	showClouds(src, res);

	return EXIT_SUCCESS;
}