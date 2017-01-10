#include "IOPLY.h"


// inherited method
int load(const char * filename, void* objectToLoad) {

	return -1;
}

int IOPLY::load(const char * filename, PointCloud::Ptr objectToLoad) {
	pcl::io::loadPLYFile<PointT>(filename, *objectToLoad);
	return 0;
}

int IOPLY::load(const char * filename, PointCloudColored::Ptr objectToLoad) {
	pcl::io::loadPLYFile<PointColorT>(filename, *objectToLoad);
	return 0;
}

int IOPLY::load(const char * filename, PointCloudNormals::Ptr objectToLoad) {
	pcl::io::loadPLYFile<PointNormalT>(filename, *objectToLoad);
	return 0;
}

int IOPLY::load(const char * filename, PointCloudFPFH::Ptr  objectToLoad) {
	pcl::io::loadPLYFile<FPFHSignature33T>(filename, *objectToLoad);
	return 0;
}

// inherited method
int IOPLY::save(const char * filename, void* objectToSave) {
	return -1;
}



int IOPLY::save(const char* filename, PointCloud::Ptr objectToSave) {
	pcl::io::savePLYFile<PointT>(filename, *objectToSave);
	return 0;
}

int IOPLY::save(const char* filename, PointCloudColored::Ptr objectToSave) {
	pcl::io::savePLYFile<PointColorT>(filename, *objectToSave);
	return 0;
}


int IOPLY::save(const char* filename, PointCloud::Ptr objectToSave, PointCloudNormals::Ptr normalsObject) {
	PointCloudNormals::Ptr tmp;
	pcl::copyPointCloud(*objectToSave, *tmp);
	pcl::copyPointCloud(*normalsObject, *tmp);
	pcl::io::savePLYFile<PointNormalT>(filename, *tmp);
	return 0;
}

int IOPLY::save(const char* filename, PointCloudFPFH::Ptr objectToSave) {
	pcl::io::savePLYFile<FPFHSignature33T>(filename, *objectToSave);
	return 0;
}