#pragma once
#include "IIO.h"

#include "pcl/io/ply_io.h"
#include "custom_typedef.h"

class IOPLY /*: public IIO*/ {

public:
	// inherited method
	int load(char * filename, void* objectToLoad);
	int save(char * filename, void* objectToSave);

	static int load(char* filename, PointCloud::Ptr objectToLoad);
	static int load(char* filename, PointCloudColored::Ptr objectToLoad);
	static int load(char* filename, PointCloudNormals::Ptr objectToLoad);
	static int load(char* filename, PointCloudFPFH::Ptr  objectToLoad);

	static int save(char* filename, PointCloud::Ptr objectToSave);
	static int save(char* filename, PointCloudColored::Ptr objectToSave);
	static int save(char* filename, PointCloud::Ptr objectToSave, PointCloudNormals::Ptr normalsObject);
	static int save(char* filename, PointCloudFPFH::Ptr objectToSave);
};