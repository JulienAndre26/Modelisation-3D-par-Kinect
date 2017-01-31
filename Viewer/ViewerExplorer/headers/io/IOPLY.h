#pragma once

#include "IIO.h"

#include "pcl/io/ply_io.h"
#include "custom_typedef.h"

#include <pcl/io/ply_io.h>
#include <pcl/io/vtk_io.h> 
#include <pcl/io/vtk_lib_io.h> 
#include <pcl/ros/conversions.h>

class IOPLY /*: public IIO*/ {

public:
	// inherited method
	int load(const char  * filename, void* objectToLoad);
	int save(const char  * filename, void* objectToSave);

	static bool load(const char * filename, PolygonMesh::Ptr mesh);
	static bool load(const char * filename, PointCloud::Ptr objectToLoad);
	static bool load(const char * filename, PointCloudColored::Ptr objectToLoad);
	static bool load(const char * filename, PointCloudNormals::Ptr objectToLoad);
	static bool load(const char * filename, PointCloudFPFH::Ptr  objectToLoad);

	static int save(const char * filename, PolygonMesh::Ptr mesh);
	static int save(const char * filename, PointCloud::Ptr objectToSave);
	static int save(const char * filename, PointCloudColored::Ptr objectToSave);
	static int save(const char * filename, PointCloud::Ptr objectToSave, PointCloudNormals::Ptr normalsObject);
	static int save(const char * filename, PointCloudFPFH::Ptr objectToSave);
};
