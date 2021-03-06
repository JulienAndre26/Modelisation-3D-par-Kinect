#pragma once

#include "./core/ICore.h"
#include "IProcessor.h"
#include <map>

#include <pcl/point_types.h>
#include <pcl/ModelCoefficients.h>
#include <pcl/filters/project_inliers.h>
#include <pcl/io/ply_io.h>
#include <pcl/io/vtk_io.h> 
#include <pcl/io/vtk_lib_io.h> 
#include <pcl/conversions.h>
#include <pcl/ros/conversions.h>


/*
Implementation of the processor interface, to access core functions from the UI.
*/

class Processor : public IProcessor {
private :
	int someField;
	ICore * core;

public:
	// inherited methods ?
	Status merge(const string& from, const string& to, const string& into);
	Status computeMetrics(float x1, float y1, float z1, float x2, float y2, float z2);
	static std::map<std::string, double>* computeBoundingBox(pcl::PolygonMesh::Ptr mesh);
	static std::map<std::string, double>* computeBoundingBox(pcl::PointCloud<pcl::PointXYZ>::Ptr cloud);
	static std::map<std::string, double>* computeBoundingBox(pcl::PointCloud<pcl::PointXYZRGB>::Ptr cloud);
	//void flatten(pcl::PointCloud<pcl::PointXYZRGB>::Ptr src, pcl::PointCloud<pcl::PointXYZRGB>::Ptr src_projected, bool isLateral);
	//static void flattenMesh(pcl::PolygonMesh::Ptr src, pcl::PolygonMesh::Ptr src_projected, bool isLateral);
};