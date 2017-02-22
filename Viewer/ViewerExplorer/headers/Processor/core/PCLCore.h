#pragma once

#include "ICore.h"
#include "custom_typedef.h"
#include "IOPLY.h"
#include "IOXML.h"
#include <vector>
#include "merge_visualizer.h"

#include <pcl/common/transforms.h>
#include <pcl/common/transformation_from_correspondences.h>
#include <pcl/filters/approximate_voxel_grid.h>

class PCLCore : public ICore {
private:
	int someattribute;

public:
	static Eigen::Matrix4f*  getMatrix(std::string path);
	static Eigen::Matrix4f getRigidTransformation(PointCloud::Ptr src, PointCloud::Ptr tgt);
	static void transformMesh(PolygonMesh::Ptr src_mesh, PointCloud::Ptr src_dotted, PointCloud::Ptr src_pick, PointCloud::Ptr tgt_pick);
	static void merge(const std::string& from, const std::string& to, const std::string& into);
	static void saveMerge(PointCloud::Ptr src_tranformed, PolygonMesh::Ptr tgt_mesh, const std::string& into);
	static void removeDuplicate(PointCloud::Ptr cloud);
	int compress(std::string* file_path);
	void* other(std::string* file_path);
	static std::vector<double> computeBoundingBoxSize(pcl::PointCloud<pcl::PointXYZ>::Ptr cloud);
	//void computeBoundingBoxSize(void * cloud);
	static int compress(pcl::PointCloud<pcl::PointXYZ>::Ptr input, pcl::PointCloud<pcl::PointXYZ>::Ptr output, float compressRatio);
};