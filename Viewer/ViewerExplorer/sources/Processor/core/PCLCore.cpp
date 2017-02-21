#include <unordered_map>

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