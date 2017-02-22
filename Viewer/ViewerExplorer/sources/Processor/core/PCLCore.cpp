#include <unordered_map>
#include <pcl/features/moment_of_inertia_estimation.h>
#include <pcl/filters/approximate_voxel_grid.h>
#include <vector>

#include "PCLCore.h"

void PCLCore::merge(const std::string& from, const std::string& to, const std::string& into) {
	//#include <iostream>
	//#include <pcl/visualization/pcl_visualizer.h>
	//#include <pcl/point_types.h>
	//#include <IOPLY.h>
	//
	//#include <stdio.h>
	//#include <string.h>
	//
	//

	//
	//pcl::PolygonMesh::Ptr src(new pcl::PolygonMesh());
	//pcl::PolygonMesh::Ptr tgt(new pcl::PolygonMesh());
	//_args src_args;
	//_args tgt_args;
	//
	//void pointPickingEventOccurred(const pcl::visualization::PointPickingEvent &event, void* args)
	//{
	//	_args* data = (_args*) args;
	//
	//	// Max elem reached (6) don't add any other element
	//	if (data->clicked_points_3d->size() >= 6)
	//		return;
	//
	//	pcl::PointXYZ point;
	//
	//	event.getPoint(point.x, point.y, point.z);
	//
	//	pcl::console::print_info("Added point : {x: %f, y: %f, z: %f}\n", point.x, point.y, point.z);
	//
	//	// Magic trick to keep only different points
	//	for (int i = 0; i < data->clicked_points_3d->size(); i++) 
	//		if ((data->clicked_points_3d->at(i).x == point.x) && 
	//			(data->clicked_points_3d->at(i).y == point.y) && 
	//			(data->clicked_points_3d->at(i).z == point.z)) 
	//			return;
	//
	//	data->clicked_points_3d->push_back(point);
	//
	//	bool baseColor;
	//	PointCloud::Ptr tmpCloud(new PointCloud);
	//
	//	// Update each colored point
	//	for (int i = 0; i < data->clicked_points_3d->size(); i++) {
	//		data->viewerPtr->removePointCloud(data->id + i);
	//		baseColor = (i / 3 == 0);
	//		tmpCloud->push_back(data->clicked_points_3d->at(i));
	//		data->viewerPtr->addPointCloud(tmpCloud, pcl::visualization::PointCloudColorHandlerCustom<PointT>(data->clicked_points_3d,
	//			((i % 3) == 0) ? (baseColor ? 210 : 50) : (baseColor ? 50 : 210),
	//			((i % 3) == 1) ? (baseColor ? 210 : 50) : (baseColor ? 50 : 210),
	//			((i % 3) == 2) ? (baseColor ? 210 : 50) : (baseColor ? 50 : 210)), data->id + i);
	//		data->viewerPtr->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 13, data->id + i);
	//		tmpCloud->clear();
	//	}
	//}
	//
	//#include <pcl/registration/icp.h>
	//Eigen::Matrix4f affineRigidTransformation(PointCloud::Ptr src, PointCloud::Ptr tgt) {
	//	// rigid transformation estimation
	//	pcl::IterativeClosestPoint<PointT, PointT> icp;
	//	icp.setInputCloud(src);
	//	icp.setInputTarget(tgt);
	//	icp.setMaxCorrespondenceDistance(0.01); // 1 centimeter, we assume that both point cloud should be very close from each other
	//	icp.setMaximumIterations(7); // should be more than enough
	//	PointCloud Final;
	//	icp.align(Final);
	//	std::cout << "has converged:" << icp.hasConverged() << " score: " <<
	//		icp.getFitnessScore() << std::endl;
	//	std::cout << icp.getFinalTransformation() << std::endl;
	//	return icp.getFinalTransformation();
	//}
	//
	//#include <pcl/common/transformation_from_correspondences.h>
	//Eigen::Matrix4f getRigidTransformation(PointCloud::Ptr src, PointCloud::Ptr tgt) {
	//	pcl::TransformationFromCorrespondences tfc;
	//
	//	// get the minimum of point picked
	//	int minSize = src_args.clicked_points_3d->size();
	//	if (src_args.clicked_points_3d->size() < tgt_args.clicked_points_3d->size())
	//		minSize = tgt_args.clicked_points_3d->size();
	//	
	//
	//	// add correspondences
	//	for (int i = 0; i < minSize; i++) {
	//		tfc.add(Eigen::Vector3f(src->at(i).x, src->at(i).y, src->at(i).z), 
	//			Eigen::Vector3f(tgt->at(i).x, tgt->at(i).y, tgt->at(i).z));
	//	}
	//
	//	// get rigid transformation
	//	return tfc.getTransformation().matrix();
	//}
	//
	//void displayResult(Eigen::Matrix4f rte) {
	//	// Transform pointcloud2
	//	PointCloud src_dotted;
	//	PointCloud tgt_dotted;
	//	pcl::fromROSMsg(src->cloud, src_dotted);
	//	pcl::fromROSMsg(tgt->cloud, tgt_dotted);
	//	// TODO : do some magic trick with this fking useless shit to keep faces (or colors)
	//	pcl::transformPointCloud(src_dotted, src_dotted, rte);
	//	// close viewers for point picking
	//	src_args.viewerPtr->close();
	//	tgt_args.viewerPtr->close();
	//	// apply icp to refine the basis transformation
	//	// pcl::transformPointCloud(src_dotted, src_dotted, affineRigidTransformation(src_dotted.makeShared(), tgt_dotted.makeShared()));
	//
	//	// result visualizer
	//	pcl::visualization::PCLVisualizer::Ptr v(new pcl::visualization::PCLVisualizer);
	//
	//	// showing target mesh
	//	v->addPolygonMesh(*tgt);
	//	// showing "shouldbeonthesameplacethanthetarget" source
	//	v->addPointCloud<pcl::PointXYZ>(src_dotted.makeShared(), "src_dotted");
	//
	//	v->spin();
	//}
	//
	//void cancelPicks(void* args) {
	//	_args* data = (_args*)args;
	//	PointCloud::Ptr tmpCloud(new PointCloud);
	//
	//	// Clear picking point cloud
	//	for (int i = 0; i < data->clicked_points_3d->size(); i++) {
	//		data->viewerPtr->removePointCloud(data->id + i);
	//		data->viewerPtr->addPointCloud(tmpCloud, data->id + i);
	//	}
	//	data->clicked_points_3d->clear();
	//}
	//
	//#include <pcl/common/transforms.h>
	//void handleKeyboardEvents(const pcl::visualization::KeyboardEvent &event, void* args){
	//	if(event.keyDown())
	//		switch (event.getKeyCode()) {
	//		case 'e':
	//			std::cout << "Bleeeeeh estimate" << std::endl;
	//			// not anough point picked
	//			if ((src_args.clicked_points_3d->size() < 4) || (tgt_args.clicked_points_3d->size() < 4))
	//				break;
	//			// Get rigid transformation estimation and display result
	//			displayResult(getRigidTransformation(src_args.clicked_points_3d, tgt_args.clicked_points_3d));
	//			break;
	//		case 'r':
	//			std::cout << "Bleeeeeh cancel" << std::endl;
	//			cancelPicks(args);
	//			break;
	//		default:
	//			break;
	//		}
	//}
	//
	//void initPickingClouds()
	//{
	//	// ----- start color stuff -----
	//	// src
	//	pcl::visualization::PointCloudColorHandlerCustom<pcl::PointXYZ> src_light_red(src_args.clicked_points_3d, 210, 50, 50); // light red
	//	// tgt
	//	pcl::visualization::PointCloudColorHandlerCustom<pcl::PointXYZ> tgt_light_red(tgt_args.clicked_points_3d, 210, 50, 50); // light red
	//	// ----- end color stuff -----
	//	// ----- igit -----
	//	src_args.viewerPtr->addPointCloud(src_args.clicked_points_3d, src_light_red, "src_pick0");
	//	src_args.viewerPtr->addPointCloud(src_args.clicked_points_3d, src_light_red, "src_pick1");
	//	src_args.viewerPtr->addPointCloud(src_args.clicked_points_3d, src_light_red, "src_pick2");
	//	src_args.viewerPtr->addPointCloud(src_args.clicked_points_3d, src_light_red, "src_pick3");
	//	src_args.viewerPtr->addPointCloud(src_args.clicked_points_3d, src_light_red, "src_pick4");
	//	src_args.viewerPtr->addPointCloud(src_args.clicked_points_3d, src_light_red, "src_pick5");
	//	tgt_args.viewerPtr->addPointCloud(tgt_args.clicked_points_3d, tgt_light_red, "tgt_pick0");
	//	tgt_args.viewerPtr->addPointCloud(tgt_args.clicked_points_3d, tgt_light_red, "tgt_pick1");
	//	tgt_args.viewerPtr->addPointCloud(tgt_args.clicked_points_3d, tgt_light_red, "tgt_pick2");
	//	tgt_args.viewerPtr->addPointCloud(tgt_args.clicked_points_3d, tgt_light_red, "tgt_pick3");
	//	tgt_args.viewerPtr->addPointCloud(tgt_args.clicked_points_3d, tgt_light_red, "tgt_pick4");
	//	tgt_args.viewerPtr->addPointCloud(tgt_args.clicked_points_3d, tgt_light_red, "tgt_pick5");
	//}
	//
	//int main(int argc, char** argv)
	//{
	//	const char* f1 = "test/plan1-removed-duplicated-closemerged.ply";
	//	const char* f2 = "test/plan2-removed-duplicated-closemerged.ply";
	//
	//	pcl::console::print_highlight("----- Loading1 -----\n");
	//	IOPLY::load(f1, src);
	//	pcl::console::print_highlight("----- Loading2 -----\n");
	//	IOPLY::load(f2, tgt);
	//
	//	pcl::console::print_highlight("----- Loaded -----\n");
	//
	//	pcl::PointCloud<pcl::PointXYZ>::Ptr src_pick(new pcl::PointCloud<pcl::PointXYZ>);
	//	pcl::PointCloud<pcl::PointXYZ>::Ptr tgt_pick(new pcl::PointCloud<pcl::PointXYZ>);
	//
	//	pcl::visualization::PCLVisualizer::Ptr viewer1(new pcl::visualization::PCLVisualizer);
	//	pcl::visualization::PCLVisualizer::Ptr viewer2(new pcl::visualization::PCLVisualizer);
	//
	//	viewer1->addPolygonMesh(*src, "src");
	//	viewer2->addPolygonMesh(*tgt, "tgt");
	//
	//	// ----- init callback args -----
	//	src_args.id = "src_pick";
	//	src_args.clicked_points_3d = src_pick;
	//	src_args.viewerPtr = viewer1;
	//	tgt_args.id = "tgt_pick";
	//	tgt_args.clicked_points_3d = tgt_pick;
	//	tgt_args.viewerPtr = viewer2;
	//
	//	// ----- init picking clouds -----
	//	initPickingClouds();
	//
	//	viewer1->registerPointPickingCallback(pointPickingEventOccurred, (void*)&src_args);
	//	viewer2->registerPointPickingCallback(pointPickingEventOccurred, (void*)&tgt_args);
	//
	//	viewer1->registerKeyboardCallback(handleKeyboardEvents, (void*)&src_args);
	//	viewer2->registerKeyboardCallback(handleKeyboardEvents, (void*)&tgt_args);
	//
	//	viewer1->spin();
	//	viewer2->spin();
	//}
}


int PCLCore::compress(std::string* file_path) {
	PointCloud::Ptr raw_cloud(new PointCloud);
	PointCloud::Ptr filtered_cloud(new PointCloud);
	IOPLY::load(file_path->c_str(), raw_cloud);
	
	pcl::ApproximateVoxelGrid<PointT> avg;
	// keeps only 1 voxel (point) within a sphere of 1cm radius
	avg.setLeafSize(0.01, 0.01, 0.01);
	avg.setInputCloud(raw_cloud);
	avg.filter(*filtered_cloud);

	IOPLY::save(file_path->c_str(), filtered_cloud);

	return 0;
}

int PCLCore::compress(pcl::PointCloud<pcl::PointXYZ>::Ptr input, pcl::PointCloud<pcl::PointXYZ>::Ptr output, float compressRatio) {
	pcl::PointCloud<pcl::PointXYZ>::Ptr copy(input);

	pcl::ApproximateVoxelGrid<PointT> avg;
	// keeps only 1 voxel (point) within a sphere of 1cm radius
	avg.setLeafSize(compressRatio, compressRatio, compressRatio);
	avg.setInputCloud(copy);
	avg.filter(*output);

	return 0;
}


void* PCLCore::other(std::string* file_path) {
	return (void*)0;
}


Eigen::Matrix4f * PCLCore::getMatrix(std::string path) {
	IOXML * parser = IOXML::Instance();
	parser->init(path);
	std::unordered_map<std::string, float> * representation = parser->getMatrix();

	Eigen::Matrix4f * result = new Eigen::Matrix4f();
	*result << representation->at(std::string("m11")),
				representation->at(std::string("m21")),
				representation->at(std::string("m31")),
				representation->at(std::string("m41")),

				representation->at(std::string("m12")),
				representation->at(std::string("m22")),
				representation->at(std::string("m32")),
				representation->at(std::string("m42")),

				representation->at(std::string("m13")),
				representation->at(std::string("m23")),
				representation->at(std::string("m33")),
				representation->at(std::string("m43")),

				representation->at(std::string("m14")),
				representation->at(std::string("m24")),
				representation->at(std::string("m34")),
				representation->at(std::string("m44"));

	return result;
} 

#include "Processor.h"
std::vector<double> PCLCore::computeBoundingBoxSize(pcl::PointCloud<pcl::PointXYZ>::Ptr cloud) {
	pcl::PointCloud<pcl::PointXYZ>::Ptr filtered(new PointCloud);
	compress(cloud, filtered, 0.1);
	pcl::MomentOfInertiaEstimation <pcl::PointXYZ> feature_extractor;
	feature_extractor.setInputCloud(filtered);
	feature_extractor.compute();

	//std::vector <float> moment_of_inertia;
	//std::vector <float> eccentricity;
	//pcl::PointXYZ min_point_AABB;
	//pcl::PointXYZ max_point_AABB;
	pcl::PointXYZ min_point_OBB;
	pcl::PointXYZ max_point_OBB;
	pcl::PointXYZ position_OBB;
	Eigen::Matrix3f rotational_matrix_OBB;
	//float major_value, middle_value, minor_value;
	Eigen::Vector3f major_vector, middle_vector, minor_vector;
	Eigen::Vector3f mass_center;

	//feature_extractor.getMomentOfInertia(moment_of_inertia);
	//feature_extractor.getEccentricity(eccentricity);
	//feature_extractor.getAABB(min_point_AABB, max_point_AABB);
	feature_extractor.getOBB(min_point_OBB, max_point_OBB, position_OBB, rotational_matrix_OBB);
	//feature_extractor.getEigenValues(major_value, middle_value, minor_value);
	//feature_extractor.getEigenVectors(major_vector, middle_vector, minor_vector);
	//feature_extractor.getMassCenter(mass_center);

	Eigen::Vector3f position(position_OBB.x, position_OBB.y, position_OBB.z);
	//Eigen::Quaternionf quat(rotational_matrix_OBB);

	//pcl::PointXYZ center(mass_center(0), mass_center(1), mass_center(2));
	//pcl::PointXYZ x_axis(major_vector(0) + mass_center(0), major_vector(1) + mass_center(1), major_vector(2) + mass_center(2));
	//pcl::PointXYZ y_axis(middle_vector(0) + mass_center(0), middle_vector(1) + mass_center(1), middle_vector(2) + mass_center(2));
	//pcl::PointXYZ z_axis(minor_vector(0) + mass_center(0), minor_vector(1) + mass_center(1), minor_vector(2) + mass_center(2));

	Eigen::Vector3f p1 (min_point_OBB.x, min_point_OBB.y, min_point_OBB.z);
	Eigen::Vector3f p2 (min_point_OBB.x, min_point_OBB.y, max_point_OBB.z);
	//Eigen::Vector3f p3 (max_point_OBB.x, min_point_OBB.y, max_point_OBB.z);
	Eigen::Vector3f p4 (max_point_OBB.x, min_point_OBB.y, min_point_OBB.z);
	Eigen::Vector3f p5 (min_point_OBB.x, max_point_OBB.y, min_point_OBB.z);
	//Eigen::Vector3f p6 (min_point_OBB.x, max_point_OBB.y, max_point_OBB.z);
	//Eigen::Vector3f p7 (max_point_OBB.x, max_point_OBB.y, max_point_OBB.z);
	//Eigen::Vector3f p8 (max_point_OBB.x, max_point_OBB.y, min_point_OBB.z);

	p1 = rotational_matrix_OBB * p1 + position;
	p2 = rotational_matrix_OBB * p2 + position;
	//p3 = rotational_matrix_OBB * p3 + position;
	p4 = rotational_matrix_OBB * p4 + position;
	p5 = rotational_matrix_OBB * p5 + position;
	//p6 = rotational_matrix_OBB * p6 + position;
	//p7 = rotational_matrix_OBB * p7 + position;
	//p8 = rotational_matrix_OBB * p8 + position;

	pcl::PointXYZ pt1 (p1 (0), p1 (1), p1 (2));
	pcl::PointXYZ pt2 (p2 (0), p2 (1), p2 (2));
	//pcl::PointXYZ pt3 (p3 (0), p3 (1), p3 (2));
	pcl::PointXYZ pt4 (p4 (0), p4 (1), p4 (2));
	pcl::PointXYZ pt5 (p5 (0), p5 (1), p5 (2));
	//pcl::PointXYZ pt6 (p6 (0), p6 (1), p6 (2));
	//pcl::PointXYZ pt7 (p7 (0), p7 (1), p7 (2));
	//pcl::PointXYZ pt8 (p8 (0), p8 (1), p8 (2));

	std::cout << pt1 << std::endl;
	std::cout << pt4 << std::endl;
	std::cout << pt5 << std::endl;

	Processor proc;
	double dist1 = proc.computeMetrics(pt1.x, pt1.y, pt1.z, pt2.x, pt2.y, pt2.z).returnedDouble;
	double dist2 = proc.computeMetrics(pt1.x, pt1.y, pt1.z, pt4.x, pt4.y, pt4.z).returnedDouble;
	double dist3 = proc.computeMetrics(pt1.x, pt1.y, pt1.z, pt5.x, pt5.y, pt5.z).returnedDouble;

	std::cout << "dist 1 : " << dist1 << std::endl;
	std::cout << "dist 2 : " << dist2 << std::endl;
	std::cout << "dist 3 : " << dist3 << std::endl;
	std::vector<double> result; //{ dist1, dist2, dist3 };
	result.push_back(dist1);
	result.push_back(dist2);
	result.push_back(dist3);
	return  result;
}

