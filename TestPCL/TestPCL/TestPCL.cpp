#include "stdafx.h"
//
//#pragma warning( disable : 4996 )
//
//#ifdef WIN32
//# define sleep(x) Sleep((x)*1000)
//#endif
//#include <boost/make_shared.hpp>
//#include <pcl/point_types.h>
//#include <pcl/point_cloud.h>
//#include <pcl/point_representation.h>
//
//#include <pcl/io/ply_io.h>
//#include <pcl/io/pcd_io.h>
//
//#include <pcl/filters/voxel_grid.h>
//#include <pcl/filters/filter.h>
//
//#include <pcl/features/normal_3d.h>
//
//#include <pcl/registration/icp.h>
//#include <pcl/registration/icp_nl.h>
//#include <pcl/registration/transforms.h>
//
//#include <pcl/visualization/pcl_visualizer.h>
//
//using pcl::visualization::PointCloudColorHandlerGenericField;
//using pcl::visualization::PointCloudColorHandlerCustom;
//
////convenient typedefs
//typedef pcl::PointXYZ PointT;
//typedef pcl::PointCloud<PointT> PointCloud;
//typedef pcl::PointXYZRGB PointColorT;
//typedef pcl::PointCloud<PointColorT> PointCloudColor;
//typedef pcl::PointNormal PointNormalT;
//typedef pcl::PointCloud<PointNormalT> PointCloudNormals;
//typedef pcl::FPFHSignature33 FPFHSignature33T;
//typedef pcl::PointCloud<FPFHSignature33T> PointCloudFPFH;
//typedef pcl::PointIndices IndicesT;
//// convenient methods
//void Load(char* filename, PointCloud::Ptr src) { pcl::io::loadPLYFile<PointT>(filename, *src); }
//void Load(char* filename, PointCloudColor::Ptr src) { pcl::io::loadPLYFile<PointColorT>(filename, *src); }
//void Load(char* filename, PointCloudNormals::Ptr src) { pcl::io::loadPCDFile<PointNormalT>(filename, *src); }
//void Load(char* filename, PointCloudFPFH::Ptr src) { pcl::io::loadPLYFile<FPFHSignature33T>(filename, *src); }
//void Save(char* filename, PointCloud::Ptr tgt) { pcl::io::savePLYFile<PointT>(filename, *tgt); }
//void Save(char* filename, PointCloudColor::Ptr tgt) { pcl::io::savePLYFile<PointColorT>(filename, *tgt); }
//void Save(char* filename, PointCloud::Ptr tgt_points, PointCloudNormals::Ptr tgt_normals) { 
//	PointCloudNormals::Ptr tmp;
//	pcl::copyPointCloud(*tgt_points, *tmp);
//	pcl::copyPointCloud(*tgt_normals, *tmp);
//	pcl::io::savePCDFile<PointNormalT>(filename, *tmp);
//}
////void SaveFeatures(char* filename, PointCloudFPFH::Ptr tgt) { pcl::io::savePLYFile<FPFHSignature33T>(filename, *tgt); }
////
////// This is a tutorial so we can afford having global variables 
//////our visualizer
////pcl::visualization::PCLVisualizer *p;
//////its left and right viewports
////int vp_1, vp_2;
////
//////convenient structure to handle our pointclouds
////struct PLY {
////	PointCloud::Ptr cloud;
////	std::string f_name;
////
////	PLY() : cloud(new PointCloud) {};
////};
////
////struct PLYComparator {
////	bool operator () (const PLY& p1, const PLY& p2) {
////		return (p1.f_name < p2.f_name);
////	}
////};
////
////
////// Define a new point representation for < x, y, z, curvature >
////class MyPointRepresentation : public pcl::PointRepresentation <PointNormalT>
////{
////	using pcl::PointRepresentation<PointNormalT>::nr_dimensions_;
////public:
////	MyPointRepresentation()
////	{
////		// Define the number of dimensions
////		nr_dimensions_ = 4;
////	}
////
////	// Override the copyToFloatArray method to define our feature vector
////	virtual void copyToFloatArray(const PointNormalT &p, float * out) const
////	{
////		// < x, y, z, curvature >
////		out[0] = p.x;
////		out[1] = p.y;
////		out[2] = p.z;
////		out[3] = p.curvature;
////	}
////};
////
////
////////////////////////////////////////////////////////////////////////////////////
//////** \brief Display source and target on the first viewport of the visualizer
////void showCloudsLeft(const PointCloud::Ptr cloud_target, const PointCloud::Ptr cloud_source)
////{
////	p->removePointCloud("vp1_target");
////	p->removePointCloud("vp1_source");
////
////	PointCloudColorHandlerCustom<PointT> tgt_h(cloud_target, 0, 255, 0);
////	PointCloudColorHandlerCustom<PointT> src_h(cloud_source, 255, 0, 0);
////	p->addPointCloud(cloud_target, tgt_h, "vp1_target", vp_1);
////	p->addPointCloud(cloud_source, src_h, "vp1_source", vp_1);
////
////	PCL_INFO("Press q to begin the registration.\n");
////	p->spin();
////}
////
////
////////////////////////////////////////////////////////////////////////////////////
//////** \brief Display source and target on the second viewport of the visualizer
////void showCloudsRight(const PointCloudNormals::Ptr cloud_target, const PointCloudNormals::Ptr cloud_source)
////{
////	p->removePointCloud("source");
////	p->removePointCloud("target");
////
////
////	PointCloudColorHandlerGenericField<PointNormalT> tgt_color_handler(cloud_target, "curvature");
////	if (!tgt_color_handler.isCapable())
////		PCL_WARN("Cannot create curvature color handler!");
////
////	PointCloudColorHandlerGenericField<PointNormalT> src_color_handler(cloud_source, "curvature");
////	if (!src_color_handler.isCapable())
////		PCL_WARN("Cannot create curvature color handler!");
////
////
////	p->addPointCloud(cloud_target, tgt_color_handler, "target", vp_2);
////	p->addPointCloud(cloud_source, src_color_handler, "source", vp_2);
////
////	p->spinOnce();
////}
////
////////////////////////////////////////////////////////////////////////////////////
//////** \brief Load a set of PLY files that we want to register together
//////* \param argc the number of arguments (pass from main ())
//////* \param argv the actual command line arguments (pass from main ())
//////* \param models the resultant vector of point cloud datasets
////void loadData(int argc, char **argv, std::vector<PLY, Eigen::aligned_allocator<PLY> > &models)
////{
////	std::string extension(".ply");
////	// Suppose the first argument is the actual test model
////	for (int i = 1; i < argc; i++)
////	{
////		std::string fname = std::string(argv[i]);
////		// Needs to be at least 5: .plot
////		if (fname.size() <= extension.size())
////			continue;
////
////		std::transform(fname.begin(), fname.end(), fname.begin(), (int(*)(int))tolower);
////
////		//check that the argument is a ply file
////		if (fname.compare(fname.size() - extension.size(), extension.size(), extension) == 0)
////		{
////			// Load the cloud and saves it into the global list of models
////			PLY m;
////			m.f_name = argv[i];
////			pcl::io::loadPLYFile(argv[i], *m.cloud);
////			//remove NAN points from the cloud
////			std::vector<int> indices;
////			pcl::removeNaNFromPointCloud(*m.cloud, *m.cloud, indices);
////
////			models.push_back(m);
////		}
////	}
////}
////
////
////////////////////////////////////////////////////////////////////////////////////
//////** \brief Align a pair of PointCloud datasets and return the result
//////* \param cloud_src the source PointCloud
//////* \param cloud_tgt the target PointCloud
//////* \param output the resultant aligned source PointCloud
//////* \param final_transform the resultant transform between source and target
////void pairAlign(const PointCloud::Ptr cloud_src, const PointCloud::Ptr cloud_tgt, PointCloud::Ptr output, Eigen::Matrix4f &final_transform, bool downsample = false)
////{
////	// Downsample for consistency and speed
////	// \note enable this for large datasets
////	PointCloud::Ptr src(new PointCloud);
////	PointCloud::Ptr tgt(new PointCloud);
////	pcl::VoxelGrid<PointT> grid;
////	if (downsample)
////	{
////		grid.setLeafSize(0.05, 0.05, 0.05);
////		grid.setInputCloud(cloud_src);
////		grid.filter(*src);
////
////		grid.setInputCloud(cloud_tgt);
////		grid.filter(*tgt);
////	}
////	else
////	{
////		src = cloud_src;
////		tgt = cloud_tgt;
////	}
////
////
////	// Compute surface normals and curvature
////	PointCloudNormals::Ptr points_with_normals_src(new PointCloudNormals);
////	PointCloudNormals::Ptr points_with_normals_tgt(new PointCloudNormals);
////
////	pcl::NormalEstimation<PointT, PointNormalT> norm_est;
////	pcl::search::KdTree<pcl::PointXYZ>::Ptr tree(new pcl::search::KdTree<pcl::PointXYZ>());
////	norm_est.setSearchMethod(tree);
////	norm_est.setKSearch(30);
////
////	norm_est.setInputCloud(src);
////	norm_est.compute(*points_with_normals_src);
////	pcl::copyPointCloud(*src, *points_with_normals_src);
////
////	norm_est.setInputCloud(tgt);
////	norm_est.compute(*points_with_normals_tgt);
////	pcl::copyPointCloud(*tgt, *points_with_normals_tgt);
////
////	// Instantiate our custom point representation (defined above) ...
////	MyPointRepresentation point_representation;
////	// ... and weight the 'curvature' dimension so that it is balanced against x, y, and z
////	float alpha[4] = { 1.0, 1.0, 1.0, 1.0 };
////	point_representation.setRescaleValues(alpha);
////
////	// Align
////	pcl::IterativeClosestPointNonLinear<PointNormalT, PointNormalT> reg;
////	reg.setTransformationEpsilon(1e-6);
////	// Set the maximum distance between two correspondences (src<->tgt) to 10cm
////	// Note: adjust this based on the size of your datasets
////	reg.setMaxCorrespondenceDistance(0.1);
////	// Set the point representation
////	reg.setPointRepresentation(boost::make_shared<const MyPointRepresentation>(point_representation));
////
////	reg.setInputSource(points_with_normals_src);
////	reg.setInputTarget(points_with_normals_tgt);
////
////	// Run the same optimization in a loop and visualize the results
////	Eigen::Matrix4f Ti = Eigen::Matrix4f::Identity(), prev, targetToSource;
////	PointCloudNormals::Ptr reg_result = points_with_normals_src;
////	reg.setMaximumIterations(10);
////	for (int i = 0; i < 30; ++i)
////	{
////		PCL_INFO("Iteration Nr. %d.\n", i);
////
////		// save cloud for visualization purpose
////		points_with_normals_src = reg_result;
////
////		// Estimate
////		reg.setInputSource(points_with_normals_src);
////		reg.align(*reg_result);
////
////		//accumulate transformation between each Iteration
////		Ti = reg.getFinalTransformation() * Ti;
////
////		//if the difference between this transformation and the previous one
////		//is smaller than the threshold, refine the process by reducing
////		//the maximal correspondence distance
////		if (fabs((reg.getLastIncrementalTransformation() - prev).sum()) < reg.getTransformationEpsilon())
////			reg.setMaxCorrespondenceDistance(reg.getMaxCorrespondenceDistance() - 0.001);
////
////		prev = reg.getLastIncrementalTransformation();
////
////		// visualize current state
////		showCloudsRight(points_with_normals_tgt, points_with_normals_src);
////	}
////
////	// Get the transformation from target to source
////	targetToSource = Ti.inverse();
////
////	// Transform target back in source frame
////	pcl::transformPointCloud(*cloud_tgt, *output, targetToSource);
////
////	p->removePointCloud("source");
////	p->removePointCloud("target");
////
////	PointCloudColorHandlerCustom<PointT> cloud_tgt_h(output, 0, 255, 0);
////	PointCloudColorHandlerCustom<PointT> cloud_src_h(cloud_src, 255, 0, 0);
////	p->addPointCloud(output, cloud_tgt_h, "target", vp_2);
////	p->addPointCloud(cloud_src, cloud_src_h, "source", vp_2);
////
////	PCL_INFO("Press q to continue the registration.\n");
////	p->spin();
////
////	p->removePointCloud("source");
////	p->removePointCloud("target");
////
////	//add the source to the transformed target
////	*output += *cloud_src;
////
////	final_transform = targetToSource;
////}
////
/////*
////int ICP() {
////	PCL_INFO("ICP METHOD");
////	// Command line simulation
////	int argc = 3;
////	char** argv;
////	argv[1] = "PourJulienColor1.ply";
////	argv[2] = "PourJulienColor2.ply";
////
////	// Load data
////	std::vector<PLY, Eigen::aligned_allocator<PLY> > data;
////	loadData(argc, argv, data);
////
////	// Check user input
////	if (data.empty())
////	{
////		PCL_ERROR("Syntax is: %s <source.ply> <target.ply> [*]", argv[0]);
////		PCL_ERROR("[*] - multiple files can be added. The registration results of (i, i+1) will be registered against (i+2), etc");
////		return (-1);
////	}
////	PCL_INFO("Loaded %d datasets.", (int)data.size());
////
////	// Create a PCLVisualizer object
////	p = new pcl::visualization::PCLVisualizer(argc, argv, "Pairwise Incremental Registration example");
////	p->createViewPort(0.0, 0, 0.5, 1.0, vp_1);
////	p->createViewPort(0.5, 0, 1.0, 1.0, vp_2);
////
////	PointCloud::Ptr result(new PointCloud), source, target;
////	Eigen::Matrix4f GlobalTransform = Eigen::Matrix4f::Identity(), pairTransform;
////
////	for (size_t i = 1; i < data.size(); ++i)
////	{
////		source = data[i - 1].cloud;
////		target = data[i].cloud;
////
////		cout << "bleh before cloud left" << endl;
////
////		// Add visualization data
////		showCloudsLeft(source, target);
////
////		cout << "bleh after cloud left" << endl;
////
////		PointCloud::Ptr temp(new PointCloud);
////		PCL_INFO("Aligning %s (%d) with %s (%d).\n", data[i - 1].f_name.c_str(), source->points.size(), data[i].f_name.c_str(), target->points.size());
////		pairAlign(source, target, temp, pairTransform, true);
////
////		//transform current pair into the global transform
////		pcl::transformPointCloud(*temp, *result, GlobalTransform);
////
////		//update the global transform
////		GlobalTransform = GlobalTransform * pairTransform;
////
////		//save aligned pair, transformed into the first cloud's frame
////		std::stringstream ss;
////		ss << i << ".ply";
////		pcl::io::savePLYFile(ss.str(), *result, true);
////	}
////	return EXIT_SUCCESS;
////}
////*/
//// ------------------------------------------------------------------------------------------------//
//
//#include <pcl/filters/voxel_grid.h>
//void reduce_size(PointCloud::Ptr cloud, PointCloud::Ptr res) {
//	std::cerr << "PointCloud before filtering: " << cloud->width << " x " << cloud->height << " so.. " 
//		<< cloud->size() << " data points (" << pcl::getFieldsList(*cloud) << ")." << endl;
//
//	// Create the filtering object
//	pcl::VoxelGrid<PointT> sor;
//	sor.setInputCloud(cloud);
//	sor.setLeafSize(0.005f, 0.005f, 0.005f);
//	sor.filter(*res);
//	
//	std::cerr << "PointCloud after filtering: " << res->width << " x " << res->height << " so.. "
//		<< res->size() << " data points (" << pcl::getFieldsList(*res) << ")." << endl;
//}
//
//void reduce_size(PointCloudColor::Ptr cloud, PointCloudColor::Ptr res) {
//	std::cerr << "PointCloud before filtering: " << cloud->width << " x " << cloud->height << " so.. "
//		<< cloud->size() << " data points (" << pcl::getFieldsList(*cloud) << ")." << endl;
//
//	// Create the filtering object
//	pcl::VoxelGrid<PointColorT> sor;
//	sor.setInputCloud(cloud);
//	sor.setLeafSize(0.005f, 0.005f, 0.005f);
//	sor.filter(*res);
//
//	std::cerr << "PointCloud after filtering: " << res->width << " x " << res->height << " so.. "
//		<< res->size() << " data points (" << pcl::getFieldsList(*res) << ")." << endl;
//}
//
//double computeCloudResolution(const PointCloud::ConstPtr &cloud)
//{
//	double res = 0.0;
//	int n_points = 0;
//	int nres;
//	std::vector<int> indices(2);
//	std::vector<float> sqr_distances(2);
//	pcl::search::KdTree<PointT> tree;
//	tree.setInputCloud(cloud);
//
//	for (size_t i = 0; i < cloud->size(); ++i)
//	{
//		if (!pcl_isfinite((*cloud)[i].x))
//		{
//			continue;
//		}
//		//Considering the second neighbor since the first is the point itself.
//		nres = tree.nearestKSearch(i, 2, indices, sqr_distances);
//		if (nres == 2)
//		{
//			res += sqrt(sqr_distances[1]);
//			++n_points;
//		}
//	}
//	if (n_points != 0)
//	{
//		res /= n_points;
//	}
//	return res;
//}
//
//#include <pcl/keypoints/iss_3d.h>
//
//void ISS(PointCloud::Ptr cloud, PointCloud::Ptr keypoints, IndicesT::Ptr indices) {
//	cout << "Cloud with size = " << cloud->size() << endl;
//	// ----- ISS keypoints -----
//	pcl::ISSKeypoint3D<PointT, PointT> iss_detector;
//	// ----- Cloud resolution -----
//	double model_resolution = computeCloudResolution(cloud);
//	// ISS parameters
//	double gamma_21(0.975);
//	double gamma_32(0.975);
//	double min_neighbors(5);
//	int threads(4);
//	double salient_radius = 6 * model_resolution;
//	double non_max_radius = 4 * model_resolution;
//	// double normal_radius = 4 * model_resolution;
//	// double border_radius = 1 * model_resolution;
//	iss_detector.setSalientRadius(salient_radius);
//	iss_detector.setNonMaxRadius(non_max_radius);
//	// iss_detector.setNormalRadius(normal_radius);
//	// iss_detector.setBorderRadius(border_radius);
//	iss_detector.setThreshold21(gamma_21);
//	iss_detector.setThreshold32(gamma_32);
//	iss_detector.setMinNeighbors(min_neighbors);
//	iss_detector.setNumberOfThreads(threads);
//	iss_detector.setInputCloud(cloud);
//	// ISS result
//	iss_detector.compute(*keypoints);
//	for(int i = 0; i < keypoints->size(); i ++)
//		indices->indices.push_back(iss_detector.getIndices()->at(i));
//	cout << "Keypoints found : " << keypoints->size() << endl;
//}
//
//#include <pcl/features/fpfh.h>
//void FPFH(PointCloud::Ptr cloud, PointCloudNormals::Ptr normals, PointCloud::Ptr keypoints, PointCloudFPFH::Ptr features) {
//	cout << "Cloud with size = " << cloud->size() << endl;
//	// ----- FPFH features -----
//	pcl::FPFHEstimation<PointT, PointNormalT, FPFHSignature33T> fpfh;
//	// FPFH parameters
//	float radius(0.05); 
//	fpfh.setSearchSurface(cloud);
//	fpfh.setInputNormals(normals);
//	fpfh.setRadiusSearch(radius);
//	fpfh.setInputCloud(keypoints);
//	// FPFH result
//	fpfh.compute(*features);
//	cout << "Features found : " << features->size() << endl;
//}
//
//#include <pcl/registration/correspondence_estimation.h>
//#include <pcl/registration/correspondence_rejection_sample_consensus.h>
//#include <pcl/registration/transformation_estimation_svd.h>
//void registration_transformation_estimation(PointCloudFPFH::Ptr source_features, PointCloudFPFH::Ptr target_features,
//	PointCloud::Ptr source_keypoints, PointCloud::Ptr target_keypoints,
//	Eigen::Matrix4f *transform) {
//	cout << "Registration" << endl;
//	// ----- Correspondences -----
//	pcl::CorrespondencesPtr correspondences(new pcl::Correspondences);
//	pcl::registration::CorrespondenceEstimation<FPFHSignature33T, FPFHSignature33T> cest;
//	// Correspondences parameters
//	cest.setInputSource(source_features);
//	cest.setInputTarget(target_features);
//	// Correspondences result
//	cest.determineCorrespondences(*correspondences);
//
//	// ----- Rejector -----
//	pcl::CorrespondencesPtr corr_filtered(new pcl::Correspondences);
//	pcl::registration::CorrespondenceRejectorSampleConsensus<PointT> rejector;
//	// Rejector parameters
//	rejector.setInputSource(source_keypoints);
//	rejector.setInputTarget(target_keypoints);
//	rejector.setInlierThreshold(0.1);
//	rejector.setMaximumIterations(1000);
//	rejector.setRefineModel(true);
//	rejector.setInputCorrespondences(correspondences);
//	// Rejector result
//	rejector.getCorrespondences(*corr_filtered);
//	
//	// ----- Transformation estimation -----
//	pcl::registration::TransformationEstimationSVD<PointT, PointT> trans_est;
//	trans_est.estimateRigidTransformation(*source_keypoints,*target_keypoints,*corr_filtered, *transform);
//}
//
//#include <pcl/features/normal_3d.h>
//void normals(PointCloud::Ptr cloud, PointCloudNormals::Ptr normals) {
//	cout << "Cloud with size = " << cloud->size() << endl;
//
//	// ----- Normal estimation -----
//	pcl::NormalEstimation<PointT, PointNormalT> ne;
//	// Normal parameters
//	ne.setInputCloud(cloud);
//	// Tree search
//	pcl::search::KdTree<PointT>::Ptr tree(new pcl::search::KdTree<PointT>());
//	ne.setSearchMethod(tree);
//	ne.setRadiusSearch(0.05);
//	// Normal result
//	ne.compute(*normals);
//	pcl::concatenateFields(*normals, *cloud, *normals);
//
//	cout << "normals found : " << normals->size() << endl;
//}
//void registration_ICP(PointCloud::Ptr src_corr, PointCloud::Ptr tgt_corr, 
//	Eigen::Matrix4f *transformation) {
//	// Align
//	pcl::IterativeClosestPoint<PointT, PointT> reg;
//	reg.setTransformationEpsilon(1e-6);
//	// Set the maximum distance between two correspondences (src<->tgt) to 10cm
//	// Note: adjust this based on the size of your datasets
//	reg.setMaxCorrespondenceDistance(0.5);
//	reg.setRANSACOutlierRejectionThreshold(0.05);
//
////	reg.setInputSource(src_corr);
//	reg.setInputTarget(tgt_corr);
//
//	// Run the same optimization in a loop and visualize the results
//	Eigen::Matrix4f Ti = Eigen::Matrix4f::Identity(), prev;
//
//	PointCloud::Ptr res(new PointCloud);
//
//	Sleep(2000);
//	// reg.setMaximumIterations(1000);
//	reg.setMaximumIterations(2);
//	for (int i = 0; i < 25; i++) {
//		showIntermediateCloud(src_corr, tgt_corr);
//
//		cout << "Iteration N°" << i << endl;
//		// Estimate
//		reg.setInputSource(src_corr);
//		reg.align(*res);
//
//		//accumulate transformation between each Iteration
//		Ti = reg.getFinalTransformation() * Ti;
//
//		//if the difference between this transformation and the previous one
//		//is smaller than the threshold, refine the process by reducing
//		//the maximal correspondence distance
//		if (fabs((reg.getLastIncrementalTransformation() - prev).sum()) < reg.getTransformationEpsilon())
//			reg.setMaxCorrespondenceDistance(reg.getMaxCorrespondenceDistance() - 0.005);
//
//		prev = reg.getLastIncrementalTransformation();
//
//		cout << "score: " << reg.getFitnessScore() << endl;
//
//		// save cloud for visualization purpose
//		src_corr = res;
//	}
//
//	// Get the transformation from target to source
//	//Ti = reg.getFinalTransformation();
//	(*transformation) = Ti.inverse();
//	// Print matrix
//	for (int i = 0; i < 4; i++) {
//		for (int j = 0; j < 4; j++)
//			cout << (*transformation)(i, j) << " | ";
//		cout << endl;
//	}
//}
//









//int main(int argc, char** argv)
//{
//	time_t  timev;
//	time(&timev);
//	
//	argv[1] = "Demo/Plan1.ply";
//	argv[2] = "Demo/Plan2.ply";
//	//// ----- Loading ply files -----
//	//PointCloud::Ptr src_raw(new PointCloud);
//	//PointCloud::Ptr tgt_raw(new PointCloud);
//
//	//// ----- Loading ply files with color -----
//	PointCloudColor::Ptr src_raw(new PointCloudColor);
//	PointCloudColor::Ptr tgt_raw(new PointCloudColor);
//
//	// ----- Sampling clouds -----
//	PointCloud::Ptr src(new PointCloud);
//	PointCloud::Ptr tgt(new PointCloud);
//	//if (boost::filesystem::exists("organized1.ply")) Load("organized1.ply", src);
//	//else { Load(argv[1], src_raw); reduce_size(src_raw, src); Save("organized1.ply", src); }
//	//if (boost::filesystem::exists("organized2.ply")) Load("organized2.ply", tgt);
//	//else { Load(argv[2], tgt_raw); reduce_size(tgt_raw, tgt); Save("organized2.ply", tgt); }
//
//	// ----- Sampling color clouds -----
//	PointCloudColor::Ptr src_color(new PointCloudColor);
//	PointCloudColor::Ptr tgt_color(new PointCloudColor);
//	if (boost::filesystem::exists("organizedcolor1.ply")) Load("organizedcolor1.ply", src_color);
//	else { Load(argv[1], src_raw); reduce_size(src_raw, src_color); Save("organizedcolor1.ply", src_color); }
///*	if (boost::filesystem::exists("organizedcolor2.ply")) Load("organizedcolor2.ply", tgt_color);
//	else { Load(argv[2], tgt_raw); reduce_size(tgt_raw, tgt_color); Save("organizedcolor2.ply", tgt_color); } */
//	registration_mock3(src_color);
//
//	// ----- Keypoints ------
//	PointCloud::Ptr src_keypoints(new PointCloud);
//	PointCloud::Ptr tgt_keypoints(new PointCloud);
//	IndicesT::Ptr src_indices(new IndicesT);
//	IndicesT::Ptr tgt_indices(new IndicesT);
///*	if (boost::filesystem::exists("keypoints1.ply")) Load("keypoints1.ply", src_keypoints);
//	else { ISS(src, src_keypoints, src_indices); Save("keypoints1.ply", src_keypoints); }
//	if (boost::filesystem::exists("keypoints2.ply")) Load("keypoints2.ply", tgt_keypoints);
//	else { ISS(tgt, tgt_keypoints, tgt_indices); Save("keypoints2.ply", tgt_keypoints); } */
//	ISS(src, src_keypoints, src_indices);
//	ISS(tgt, tgt_keypoints, tgt_indices);
//
//	// ----- Normals -----
//	PointCloudNormals::Ptr src_normals_raw(new PointCloudNormals);
//	PointCloudNormals::Ptr tgt_normals_raw(new PointCloudNormals);
//	PointCloudNormals::Ptr src_normals(new PointCloudNormals);
//	PointCloudNormals::Ptr tgt_normals(new PointCloudNormals);
///*	if (boost::filesystem::exists("normals1.pcd")) Load("normals1.pcd", src_normals);
//	else { normals(src_keypoints, src_normals); Save("normals1.pcd", src_keypoints, src_normals); }
//	if (boost::filesystem::exists("normals2.pcd")) Load("normals2.pcd", tgt_normals);
//	else { normals(tgt_keypoints, tgt_normals); Save("normals2.pcd", tgt_keypoints, tgt_normals); } */
///*	normals(src, src_normals_raw);
//	normals(tgt, tgt_normals_raw);
//
//	for (int i = 0; i < src_indices->indices.size(); i++)
//		src_normals->push_back(
//			src_normals_raw->at(
//				src_indices->indices.at(i)));
//	for (int i = 0; i < tgt_indices->indices.size(); i++)
//		tgt_normals->push_back(
//			tgt_normals_raw->at(
//				tgt_indices->indices.at(i))); */
//
//	// ----- Features ------
//	/* PointCloudFPFH::Ptr src_features(new PointCloudFPFH);
//	PointCloudFPFH::Ptr tgt_features(new PointCloudFPFH);
//	if (boost::filesystem::exists("features1.ply")) LoadFeatures("features1.ply", src_features);
//	else { FPFH(src, src_normals, src_keypoints, src_features); SaveFeatures("features1.ply", src_features); }
//	if (boost::filesystem::exists("features2.ply")) LoadFeatures("features2.ply", tgt_features);
//	else { FPFH(tgt, tgt_normals, tgt_keypoints, tgt_features); SaveFeatures("features2.ply", tgt_features); } */
//
//	// ----- Correspondences -----
//	pcl::CorrespondencesPtr correspondences(new pcl::Correspondences);
//	pcl::registration::CorrespondenceEstimation<PointT, PointT> cest;
///* 	PointCloud::Ptr src_corr(new PointCloud), tgt_corr(new PointCloud);
//	// Correspondences parameters
//	cest.setInputSource(src_keypoints);
//	cest.setInputTarget(tgt_keypoints);
//	// Correspondences result
//	// cest.determineCorrespondences(*correspondences);
//	cest.determineReciprocalCorrespondences(*correspondences);
//	float distance_max = 0;
//	showCorrespondences(src_keypoints, tgt_keypoints, src_corr, tgt_corr, correspondences, distance_max); */
//
//	// ----- Registration ------
//	Eigen::Matrix4f transformation;
//	// Estimated transformation
//	// registration_transformation_estimation(src_features, tgt_features, src_keypoints, tgt_keypoints, &transformation);
//	PointCloud::Ptr res(new PointCloud);
//	registration_ICP(src_keypoints, tgt_keypoints, &transformation);
//	Eigen::Matrix4f inverse = transformation.inverse();
//	
//	/* PointCloud::Ptr res_xyz(new PointCloud);
//	for (int i = 0; i < res->size(); i++)
//		res_xyz->push_back(PointT(res->at(i).x, res->at(i).y, res->at(i).z)); */
//
//	pcl::transformPointCloud(*tgt, *res, transformation);
//	pcl::transformPointCloud(*tgt, *tgt, inverse);
//	
//	time_t  timev2;
//	time(&timev2);
//	cout << "Total duration: " << timev2 - timev << endl;
//	// ----- Show clouds -----
//	showClouds(src, tgt, res, src_keypoints, src_normals, tgt_keypoints, tgt_normals);
//
//	return EXIT_SUCCESS;
//}
