#include "IOPLY.h"


// inherited method
int load(char * filename, void* objectToLoad) {

	return -1;
}

int IOPLY::load(char * filename, PointCloud::Ptr objectToLoad) {
	pcl::io::loadPLYFile<PointT>(filename, *objectToLoad);
	return 0;
}

int IOPLY::load(char * filename, PointCloudColored::Ptr objectToLoad) {
	pcl::io::loadPLYFile<PointColorT>(filename, *objectToLoad);
	return 0;
}

int IOPLY::load(char * filename, PointCloudNormals::Ptr objectToLoad) {
	pcl::io::loadPLYFile<PointNormalT>(filename, *objectToLoad);
	return 0;
}

int IOPLY::load(char * filename, PointCloudFPFH::Ptr  objectToLoad) {
	pcl::io::loadPLYFile<FPFHSignature33T>(filename, *objectToLoad);
	return 0;
}

// inherited method
int IOPLY::save(char * filename, void* objectToSave) {
	return -1;
}



int IOPLY::save(char* filename, PointCloud::Ptr objectToSave) {
	pcl::io::savePLYFile<PointT>(filename, *objectToSave);
	return 0;
}

int IOPLY::save(char* filename, PointCloudColored::Ptr objectToSave) {
	pcl::io::savePLYFile<PointColorT>(filename, *objectToSave);
	return 0;
}


int IOPLY::save(char* filename, PointCloud::Ptr objectToSave, PointCloudNormals::Ptr normalsObject) {
	PointCloudNormals::Ptr tmp;
	pcl::copyPointCloud(*objectToSave, *tmp);
	pcl::copyPointCloud(*normalsObject, *tmp);
	pcl::io::savePLYFile<PointNormalT>(filename, *tmp);
	return 0;
}

int IOPLY::save(char* filename, PointCloudFPFH::Ptr objectToSave) {
	pcl::io::savePLYFile<FPFHSignature33T>(filename, *objectToSave);
	return 0;
}