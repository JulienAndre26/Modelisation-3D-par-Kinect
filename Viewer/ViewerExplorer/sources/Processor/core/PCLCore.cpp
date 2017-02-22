#include <unordered_map>
#include <pcl/features/moment_of_inertia_estimation.h>
#include <pcl/filters/approximate_voxel_grid.h>
#include <vector>

#include "PCLCore.h"

Eigen::Matrix4f PCLCore::getRigidTransformation(PointCloud::Ptr src, PointCloud::Ptr tgt) {
	pcl::TransformationFromCorrespondences tfc;

	// get the minimum of point picked
	int minSize = src->size();
	if (src->size() < tgt->size())
		minSize = tgt->size();
	
	// add correspondences
	for (int i = 0; i < minSize; i++) {
		tfc.add(Eigen::Vector3f(src->at(i).x, src->at(i).y, src->at(i).z), 
			Eigen::Vector3f(tgt->at(i).x, tgt->at(i).y, tgt->at(i).z));
	}

	// get rigid transformation
	return tfc.getTransformation().matrix();
}

// WORK IN STAND BY
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

void PCLCore::transformMesh(PolygonMesh::Ptr src_mesh, PointCloud::Ptr src_dotted, PointCloud::Ptr src_pick, PointCloud::Ptr tgt_pick){ // WORK IN STAND BY : PolygonMesh::Ptr tgt_mesh
	// TODO : do some magic trick with this fking useless shit to keep faces (or colors at least)
	// Transform pointcloud2
	pcl::fromROSMsg(src_mesh->cloud, *src_dotted);
	pcl::transformPointCloud(*src_dotted, *src_dotted, PCLCore::getRigidTransformation(src_pick, tgt_pick));
	// WORK IN STAND BY
	// apply icp to refine the basis transformation
	// PointCloud tgt_dotted;
	// pcl::fromROSMsg(tgt_mesh->cloud, tgt_dotted);
	// pcl::transformPointCloud(src_dotted, src_dotted, affineRigidTransformation(src_dotted.makeShared(), tgt_dotted.makeShared()));
}

void PCLCore::merge(const std::string& from, const std::string& to, const std::string& into) {
	MergeVisualizer(from, to, into);
}

void PCLCore::saveMerge(PointCloud::Ptr src_transformed, PolygonMesh::Ptr tgt_mesh, const std::string& into) {
	PointCloud::Ptr tgt_dotted(new PointCloud);
	// Remove source duplication
	PCLCore::removeDuplicate(src_transformed);
	// Add tgt points
	pcl::fromROSMsg(tgt_mesh->cloud, *tgt_dotted);
	// Remove target duplication
	PCLCore::removeDuplicate(tgt_dotted);
	// Concatenate data
	*src_transformed += *tgt_dotted;
	// Remove result duplication (may append)
	PCLCore::removeDuplicate(src_transformed);

	PolygonMesh::Ptr resulting_mesh(new PolygonMesh);
	pcl::toROSMsg(*src_transformed, resulting_mesh->cloud);

	// Save merged points
	IOPLY::save(into.c_str(), resulting_mesh);
}

// sorting functions
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

void PCLCore::removeDuplicate(PointCloud::Ptr cloud) {
	std::sort(cloud->points.begin(), cloud->points.end(), comparePoint);
	cloud->points.erase(std::unique(cloud->points.begin(), cloud->points.end(), equalPoint), cloud->points.end());
	cloud->width = cloud->points.size();
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

