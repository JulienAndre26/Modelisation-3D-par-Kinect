#pragma once

#include "ICore.h"
#include "custom_typedef.h"
#include "IOPLY.h"

#include <pcl/filters/approximate_voxel_grid.h>

class PCLCore : public ICore {
private:
	int someattribute;

public:
	static Eigen::Matrix4f*  getMatrix(std::string path);
	void* merge(std::string* file_path_from, std::string* file_path_to);
	int compress(std::string* file_path);
	void* other(std::string* file_path);
};