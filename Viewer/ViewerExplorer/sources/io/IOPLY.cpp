#include "IOPLY.h"

// inherited method
bool load(const char * filename, void* objectToLoad) {
	return false;
}

bool IOPLY::load(const char * filename, PolygonMesh::Ptr mesh) {
	if (!IIO::fileExists(std::string(filename)))
		return false;

	pcl::io::loadPolygonFile(filename, *mesh);
	return true;
}

bool IOPLY::load(const char * filename, PointCloud::Ptr objectToLoad) {
	if (!IIO::fileExists(std::string(filename)))
		return false;
	//std::cout << "file exists" << std::endl;
	pcl::PolygonMesh::Ptr mesh(new pcl::PolygonMesh());
	pcl::io::loadPolygonFile(filename, *mesh);
	//std::cout << "polygon mesh created" << std::endl;

	//pcl::PCLPointCloud2 mesh_cloud2 = mesh->cloud;
	pcl::fromROSMsg(mesh->cloud, *objectToLoad);
	//std::cout << "conversion done" << std::endl;
	return true;
}

bool IOPLY::load(const char * filename, PointCloudColored::Ptr objectToLoad) {
	if (!IIO::fileExists(std::string(filename)))
		return false;

	pcl::PolygonMesh::Ptr mesh(new pcl::PolygonMesh);
	pcl::io::loadPolygonFile(filename, *mesh);

	pcl::PCLPointCloud2 mesh_cloud2 = mesh->cloud;
	pcl::fromROSMsg(mesh_cloud2, *objectToLoad);
	return true;
}

bool IOPLY::load(const char * filename, PointCloudNormals::Ptr objectToLoad) {
	if (!IIO::fileExists(std::string(filename)))
		return false;

	pcl::PolygonMesh::Ptr mesh(new pcl::PolygonMesh);
	pcl::io::loadPolygonFile(filename, *mesh);

	pcl::PCLPointCloud2 mesh_cloud2 = mesh->cloud;
	pcl::fromROSMsg(mesh_cloud2, *objectToLoad);
	return true;
}

bool IOPLY::load(const char * filename, PointCloudFPFH::Ptr  objectToLoad) {
	if (!IIO::fileExists(std::string(filename)))
		return false;

	pcl::PolygonMesh::Ptr mesh(new pcl::PolygonMesh);
	pcl::io::loadPolygonFile(filename, *mesh);

	pcl::PCLPointCloud2 mesh_cloud2 = mesh->cloud;
	pcl::fromROSMsg(mesh_cloud2, *objectToLoad);
	return true;
}

// inherited method
int IOPLY::save(const char * filename, void* objectToSave) {
	return -1;
}

int IOPLY::save(const char * filename, PolygonMesh::Ptr mesh)
{
	pcl::io::savePolygonFile(filename, *mesh);
	return 0;
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