#pragma once

#include "IIO.h"

#include "pcl/io/ply_io.h"
#include "custom_typedef.h"

class IOPLY /*: public IIO*/ {

public:
	// inherited method
	int load(const char  * filename, void* objectToLoad);
	int save(const char  * filename, void* objectToSave);

	static int load(const char * filename, PointCloud::Ptr objectToLoad);
	static int load(const char * filename, PointCloudColored::Ptr objectToLoad);
	static int load(const char * filename, PointCloudNormals::Ptr objectToLoad);
	static int load(const char * filename, PointCloudFPFH::Ptr  objectToLoad);

	static int save(const char * filename, PointCloud::Ptr objectToSave);
	static int save(const char * filename, PointCloudColored::Ptr objectToSave);
	static int save(const char * filename, PointCloud::Ptr objectToSave, PointCloudNormals::Ptr normalsObject);
	static int save(const char * filename, PointCloudFPFH::Ptr objectToSave);
};
