//#pragma once
//#include "custom_typedef.h"
//
//class CallbackArgs {
//
//private:
//	PointCloud::Ptr clicked_points_3d;
//	pcl::visualization::PCLVisualizer::Ptr viewerPtr;
//	char * distance;
//
//public:
//	CallbackArgs() {
//		this->distance = new char[128];
//		sprintf(this->distance, "%f", 0);
//	}
//	CallbackArgs(PointCloud::Ptr cloud, Visualizer::Ptr viewer, char * dist) :
//		clicked_points_3d(cloud),
//		viewerPtr(viewer),
//		distance(dist)
//	{
//		sprintf(this->distance, "%f", 0);
//	}
//
//
//	PointCloud::Ptr getPointCloud() {
//		return this->clicked_points_3d;
//	}
//
//	Visualizer::Ptr getViewer() {
//		return this->viewerPtr;
//	}
//
//	char * getDist() {
//		return this->distance;
//	}
//
//
//	void setPointCloud(PointCloud::Ptr cloud) {
//		this->clicked_points_3d = cloud;
//	}
//
//	void setViewer(MetricVisualizer * viewer, char* framename) {
//		boost::shared_ptr<Visualizer> viewer_;
//		viewer_.reset(new Visualizer(*viewer));
//		this->viewerPtr = viewer_;
//	}
//
//	void setDist(char * dist) {
//		this->distance = dist;
//	}
//
//	
//};