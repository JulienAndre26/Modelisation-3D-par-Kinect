#include "stdafx.h"
//
//#pragma warning( disable : 4996 )
//
//#ifdef WIN32
//# define sleep(x) Sleep((x)*1000)
//#endif
//
//using namespace std;
//
////#include <boost/make_shared.hpp>
//#include <pcl/point_types.h>
//#include <pcl/point_cloud.h>
//
//#include <pcl/io/ply_io.h>
//#include <pcl/io/pcd_io.h>
//
//#include <pcl/common/time.h>
//#include <pcl/console/print.h>
////convenient typedefs
//typedef pcl::PointXYZ PointT;
//typedef pcl::PointCloud<PointT> PointCloud;
//typedef pcl::PointNormal PointNormalT;
//typedef pcl::PointCloud<PointNormalT> PointCloudNormals;
//typedef pcl::PointIndices IndicesT;
//// convenient methods
//void Load(char* filename, PointCloud::Ptr src) { pcl::io::loadPLYFile<PointT>(filename, *src); }
//void Load(char* filename, PointCloudNormals::Ptr src) { pcl::io::loadPCDFile<PointNormalT>(filename, *src); }
//void Save(char* filename, PointCloud::Ptr tgt) { pcl::io::savePLYFile<PointT>(filename, *tgt); }
//void Save(char* filename, PointCloud::Ptr tgt_points, PointCloudNormals::Ptr tgt_normals) {
//	PointCloudNormals::Ptr tmp;
//	pcl::copyPointCloud(*tgt_points, *tmp);
//	pcl::copyPointCloud(*tgt_normals, *tmp);
//	pcl::io::savePCDFile<PointNormalT>(filename, *tmp);
//}
//
//#include <pcl/filters/approximate_voxel_grid.h>
//void reduce_size(PointCloud::Ptr cloud, PointCloud::Ptr res) {
//	std::cerr << "PointCloud before filtering: " << cloud->width << " x " << cloud->height << " so.. "
//		<< cloud->size() << " data points (" << pcl::getFieldsList(*cloud) << ")." << endl;
//
//	// Create the filtering object
//	pcl::ApproximateVoxelGrid<pcl::PointXYZ> avg;
//	avg.setLeafSize(0.02f, 0.02f, 0.02f);
//	avg.setInputCloud(cloud);
//	avg.filter(*res);
//
//	std::cerr << "PointCloud after filtering: " << res->width << " x " << res->height << " so.. "
//		<< res->size() << " data points (" << pcl::getFieldsList(*res) << ")." << endl;
//}
//
//#include <pcl/registration/ndt.h>
//void _align(PointCloud::Ptr src, PointCloud::Ptr tgt,
//	PointCloud::Ptr res, Eigen::Matrix4f* src_to_tgt) {
//	// Initializing Normal Distributions Transform (NDT).
//	pcl::NormalDistributionsTransform<PointT, PointT> ndt;
//
//	// ----- Setting scale dependent NDT parameters -----
//	// Setting minimum transformation difference for termination condition.
//	ndt.setTransformationEpsilon(0.001);
//	// Setting maximum step size for More-Thuente line search.
//	ndt.setStepSize(0.1);
//	//Setting Resolution of NDT grid structure (VoxelGridCovariance).
//	ndt.setResolution(1.0);
//	// Setting max number of registration iterations.
//	ndt.setMaximumIterations(20);
//
//	// Setting point cloud to be aligned.
//	ndt.setInputSource(src);
//	// Setting point cloud to be aligned to.
//	ndt.setInputTarget(tgt);
//
//	cout << "aligning ?" << endl;
//
//	// ----- Calculating required rigid transform to align the input cloud to the target cloud. -----
//	ndt.align(*res);
//
//	std::cout << "Normal Distributions Transform has converged:" << ndt.hasConverged()
//		<< " score: " << ndt.getFitnessScore() << std::endl;
//
//	(* src_to_tgt) = ndt.getFinalTransformation();
//}
//
//#include <pcl/visualization/pcl_visualizer.h>
//void showRes(PointCloud::Ptr src, PointCloud::Ptr tgt, PointCloud::Ptr res) {
//	// ----- Initializing point cloud visualizer -----
//	pcl::visualization::PCLVisualizer::Ptr pv(new pcl::visualization::PCLVisualizer);
//	pv->setBackgroundColor(0, 0, 0);
//	pv->setShowFPS(true);
//
//	// ----- Color declaration -----
//	pcl::visualization::PointCloudColorHandlerCustom<PointT> tgt_color(tgt, 210, 50, 50); // red
//	pcl::visualization::PointCloudColorHandlerCustom<PointT> res_color(res, 50, 210, 50); // green
//	// ----- Point cloud attachement -----
//	pv->addPointCloud(tgt, tgt_color, "target");
//	pv->addPointCloud(res, res_color, "result");
//	// ----- Rendering properties -----
//	pv->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE,	2, "target");
//	pv->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 2, "result");
//
//	// ----- Running visualizer -----
//	pv->addCoordinateSystem(1.0, "global");
//	pv->initCameraParameters();
//	while ( ! pv->wasStopped())	{
//		pv->spinOnce(100);
//	}
//}
//
//int main(int argc, char** argv)
//{
//	time_t  timev;
//	time(&timev);
//
//	pcl::ScopeTime t1("running");
//
//	argv[1] = "Demo/Plan1.ply";
//	argv[2] = "Demo/Plan2.ply";
//	// ----- Loading ply files -----
//	PointCloud::Ptr src_raw(new PointCloud);
//	PointCloud::Ptr tgt_raw(new PointCloud);
//
//	// ----- Sampling clouds -----
//	PointCloud::Ptr src(new PointCloud);
//	PointCloud::Ptr tgt(new PointCloud);
//	pcl::console::print_highlight("Loading point clouds...\n");
//	if (boost::filesystem::exists("organized1.ply")) Load("organized1.ply", src);
//	else { Load(argv[1], src_raw); reduce_size(src_raw, src); Save("organized1.ply", src); }
//	if (boost::filesystem::exists("organized2.ply")) Load("organized2.ply", tgt);
//	else { Load(argv[2], tgt_raw); reduce_size(tgt_raw, tgt); Save("organized2.ply", tgt); }
//
//	pcl::console::print_info("[CLOUD] Source cloud size : %d\n", src->size());
//	pcl::console::print_info("[CLOUD] Target cloud size : %d\n", tgt->size());
//	pcl::console::print_info("[TIME] Time spent : %3.3fs\n", t1.getTime()/1000);
//	t1.reset();
//
//	// ----- Result stocking -----
//	PointCloud::Ptr res(new PointCloud);
//	Eigen::Matrix4f src_to_tgt;
//
//	// ----- Estimating transformation -----
//	pcl::console::print_highlight("Aligning...\n");
//	_align(src, tgt, res, &src_to_tgt);
//	pcl::console::print_info("[TIME] Time spent : %3.3fs\n", t1.getTime()/1000);
//
//
//	// Transforming unfiltered, input cloud using found transform.
//	//pcl::transformPointCloud(*input_cloud, *res, ndt.getFinalTransformation());
//
//	// ----- Viewing result -----
//	showRes(src, tgt, res);
//
//	return EXIT_SUCCESS;
//}
