#include "stdafx.h"

#pragma warning( disable : 4996 )

#ifdef WIN32
# define sleep(x) Sleep((x)*1000)
#endif

#include <pcl/console/print.h>
#include <pcl/registration/correspondence_estimation.h>
#include <pcl/registration/correspondence_rejection_one_to_one.h>

#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
//convenient typedefs
typedef pcl::PointXYZ PointT;
typedef pcl::PointCloud<PointT> PointCloud;
typedef pcl::Correspondences Corresp;
typedef boost::shared_ptr<Corresp> CorrespPtr;

#include <pcl/io/ply_io.h>
#include <pcl/io/vtk_io.h> 
#include <pcl/io/vtk_lib_io.h> 
#include <pcl/ros/conversions.h>
int load(const char * filename, PointCloud::Ptr objectToLoad) {
	pcl::PolygonMesh::Ptr mesh(new pcl::PolygonMesh);
	pcl::io::loadPolygonFile(filename, *mesh);

	pcl::PCLPointCloud2 mesh_cloud2 = mesh->cloud;
	pcl::fromROSMsg(mesh_cloud2, *objectToLoad);
	return 0;
}

#include <pcl/search/kdtree.h>
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
		if (!pcl_isfinite((*cloud)[i].x)) { continue; }
		//Considering the second neighbor since the first is the point itself.
		nres = tree.nearestKSearch(i, 2, indices, sqr_distances);
		if (nres == 2)
		{
			res += sqrt(sqr_distances[1]);
			++n_points;
		}
	}
	if (n_points != 0) { res /= n_points; }
	return res;
}


#include <pcl/keypoints/iss_3d.h>
void ISS(PointCloud::Ptr cloud, PointCloud::Ptr keypoints) {
	pcl::console::print_info("Cloud with size = %d\n", cloud->size());
	// ----- ISS keypoints -----
	pcl::ISSKeypoint3D<PointT, PointT> iss;
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
	iss.setSalientRadius(salient_radius);
	iss.setNonMaxRadius(non_max_radius);
	// iss.setNormalRadius(normal_radius);
	// iss.setBorderRadius(border_radius);
	iss.setThreshold21(gamma_21);
	iss.setThreshold32(gamma_32);
	iss.setMinNeighbors(min_neighbors);
	iss.setNumberOfThreads(threads);
	iss.setInputCloud(cloud);
	// ISS result
	iss.compute(*keypoints);
	pcl::console::print_info("Keypoints found : %d\n", keypoints->size());
}

void getGoodCorrespOTO(PointCloud::Ptr srck, PointCloud::Ptr tgtk, PointCloud::Ptr srcc, PointCloud::Ptr tgtc) {
	// ----- Calculating correspondences -----
	pcl::registration::CorrespondenceEstimation<PointT, PointT> ce;
	ce.setInputSource(srck);
	ce.setInputTarget(tgtk);
	CorrespPtr matching(new Corresp);
	ce.determineCorrespondences(*matching);

	// ----- Rejecting bad correspondences -----
	CorrespPtr res_oto(new Corresp);
	pcl::registration::CorrespondenceRejectorOneToOne croto;
	croto.setInputCorrespondences(matching);
	croto.getCorrespondences(*res_oto);

	for (int i = 0; i < res_oto->size(); i++) {
		if (res_oto->at(i).index_match != -1) {
			srcc->push_back(srck->at(res_oto->at(i).index_query));
			tgtc->push_back(tgtk->at(res_oto->at(i).index_match));
		}
	}
}

#include <pcl/visualization/pcl_visualizer.h>
void showRes(PointCloud::Ptr src, PointCloud::Ptr tgt, PointCloud::Ptr src_keypoints, PointCloud::Ptr tgt_keypoints) {
	// ----- Initializing point cloud visualizer -----
	pcl::visualization::PCLVisualizer::Ptr pv(new pcl::visualization::PCLVisualizer);
	pv->setBackgroundColor(0, 0, 0);
	pv->setShowFPS(true);

	int v1(0);
	pv->createViewPort(0.0, 0.0, 0.5, 1.0, v1);
	pv->setBackgroundColor(0.2, 0.2, 0.2, v1);

	int v2(1);
	pv->createViewPort(0.5, 0.0, 1.0, 1.0, v2);
	pv->setBackgroundColor(0.2, 0.2, 0.2, v2);

	// ----- Color declaration -----
	pcl::visualization::PointCloudColorHandlerCustom<PointT> src_color(src, 210, 50, 50); // red
	pcl::visualization::PointCloudColorHandlerCustom<PointT> tgt_color(tgt, 50, 210, 50); // green
	pcl::visualization::PointCloudColorHandlerCustom<PointT> srck_color(src_keypoints, 50, 210, 210); // inverse red
	pcl::visualization::PointCloudColorHandlerCustom<PointT> tgtk_color(tgt_keypoints, 210, 50, 210); // inverse green

	// ----- Point cloud attachement -----
	pv->addPointCloud(src, src_color, "source", v1);
	pv->addPointCloud(src_keypoints, srck_color, "srck", v1);
	pv->addPointCloud(tgt, tgt_color, "target", v2);
	pv->addPointCloud(tgt_keypoints, tgtk_color, "tgtk", v2);

	// ----- Rendering properties -----
	pv->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 2, "source", v1);
	pv->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 10, "srck", v1);
	pv->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 2, "target", v2);
	pv->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 10, "tgtk", v2);

	// ----- Running visualizer -----
	pv->addCoordinateSystem(1.0, "global");
	pv->initCameraParameters();
	while ( ! pv->wasStopped())	{
		pv->spinOnce(100);
	}
}

int main(int argc, char** argv)
{
	argv[1] = "Demo/Plan1-removed-duplicated-closemerged.ply";
	argv[2] = "Demo/Plan2-removed-duplicated-closemerged.ply";
	
	// ----- Loading ply files -----
	PointCloud::Ptr src(new PointCloud);
	PointCloud::Ptr tgt(new PointCloud);
	load(argv[1], src);
	pcl::console::print_highlight("SOURCE FILE LOADED\n");
	load(argv[2], tgt);
	pcl::console::print_highlight("TARGET FILE LOADED\n");


	// ----- Calculating ISS keypoints -----
	PointCloud::Ptr src_keypoints(new PointCloud);
	PointCloud::Ptr tgt_keypoints(new PointCloud);
	ISS(src, src_keypoints);
	ISS(tgt, tgt_keypoints);

	// ----- Getting correspondances -----
	PointCloud::Ptr src_corresp(new PointCloud);
	PointCloud::Ptr tgt_corresp(new PointCloud);
	getGoodCorrespOTO(src_keypoints, tgt_keypoints, src_corresp, tgt_corresp);

	// ----- Showing result -----
	showRes(src, tgt, src_corresp, tgt_corresp);
}