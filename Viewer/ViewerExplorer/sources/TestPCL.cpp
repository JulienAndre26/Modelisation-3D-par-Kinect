//#include <iostream>
//#include <pcl/visualization/pcl_visualizer.h>
//#include <pcl/point_types.h>
//#include <IOPLY.h>
//
//#include <stdio.h>
//#include <string.h>
//
//
//typedef struct {
//	// structure used to pass arguments to the callback function
//	const char* id;
//	PointCloud::Ptr clicked_points_3d;
//	Visualizer::Ptr viewerPtr;
//	// MainWindow * ui;
//} _args;
//
//pcl::PolygonMesh::Ptr src(new pcl::PolygonMesh());
//pcl::PolygonMesh::Ptr tgt(new pcl::PolygonMesh());
//_args src_args;
//_args tgt_args;
//
//void pointPickingEventOccurred(const pcl::visualization::PointPickingEvent &event, void* args)
//{
//	_args* data = (_args*) args;
//
//	// Max elem reached (6) don't add any other element
//	if (data->clicked_points_3d->size() >= 6)
//		return;
//
//	pcl::PointXYZ point;
//
//	event.getPoint(point.x, point.y, point.z);
//
//	pcl::console::print_info("Added point : {x: %f, y: %f, z: %f}\n", point.x, point.y, point.z);
//
//	// Magic trick to keep only different points
//	for (int i = 0; i < data->clicked_points_3d->size(); i++) 
//		if ((data->clicked_points_3d->at(i).x == point.x) && 
//			(data->clicked_points_3d->at(i).y == point.y) && 
//			(data->clicked_points_3d->at(i).z == point.z)) 
//			return;
//
//	data->clicked_points_3d->push_back(point);
//
//	bool baseColor;
//	PointCloud::Ptr tmpCloud(new PointCloud);
//
//	// Update each colored point
//	for (int i = 0; i < data->clicked_points_3d->size(); i++) {
//		data->viewerPtr->removePointCloud(data->id + i);
//		baseColor = (i / 3 == 0);
//		tmpCloud->push_back(data->clicked_points_3d->at(i));
//		data->viewerPtr->addPointCloud(tmpCloud, pcl::visualization::PointCloudColorHandlerCustom<PointT>(data->clicked_points_3d,
//			((i % 3) == 0) ? (baseColor ? 210 : 50) : (baseColor ? 50 : 210),
//			((i % 3) == 1) ? (baseColor ? 210 : 50) : (baseColor ? 50 : 210),
//			((i % 3) == 2) ? (baseColor ? 210 : 50) : (baseColor ? 50 : 210)), data->id + i);
//		data->viewerPtr->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 13, data->id + i);
//		tmpCloud->clear();
//	}
//}
//
//#include <pcl/registration/icp.h>
//Eigen::Matrix4f affineRigidTransformation(PointCloud::Ptr src, PointCloud::Ptr tgt) {
//	// rigid transformation estimation
//	pcl::IterativeClosestPoint<PointT, PointT> icp;
//	icp.setInputCloud(src);
//	icp.setInputTarget(tgt);
//	icp.setMaxCorrespondenceDistance(0.01); // 1 centimeter, we assume that both point cloud should be very close from each other
//	icp.setMaximumIterations(7); // should be more than enough
//	PointCloud Final;
//	icp.align(Final);
//	std::cout << "has converged:" << icp.hasConverged() << " score: " <<
//		icp.getFitnessScore() << std::endl;
//	std::cout << icp.getFinalTransformation() << std::endl;
//	return icp.getFinalTransformation();
//}
//
//#include <pcl/common/transformation_from_correspondences.h>
//Eigen::Matrix4f getRigidTransformation(PointCloud::Ptr src, PointCloud::Ptr tgt) {
//	pcl::TransformationFromCorrespondences tfc;
//
//	// get the minimum of point picked
//	int minSize = src_args.clicked_points_3d->size();
//	if (src_args.clicked_points_3d->size() < tgt_args.clicked_points_3d->size())
//		minSize = tgt_args.clicked_points_3d->size();
//	
//
//	// add correspondences
//	for (int i = 0; i < minSize; i++) {
//		tfc.add(Eigen::Vector3f(src->at(i).x, src->at(i).y, src->at(i).z), 
//			Eigen::Vector3f(tgt->at(i).x, tgt->at(i).y, tgt->at(i).z));
//	}
//
//	// get rigid transformation
//	return tfc.getTransformation().matrix();
//}
//
//void displayResult(Eigen::Matrix4f rte) {
//	// Transform pointcloud2
//	PointCloud src_dotted;
//	PointCloud tgt_dotted;
//	pcl::fromROSMsg(src->cloud, src_dotted);
//	pcl::fromROSMsg(tgt->cloud, tgt_dotted);
//	// TODO : do some magic trick with this fking useless shit to keep faces (or colors)
//	pcl::transformPointCloud(src_dotted, src_dotted, rte);
//	// close viewers for point picking
//	src_args.viewerPtr->close();
//	tgt_args.viewerPtr->close();
//	// apply icp to refine the basis transformation
//	// pcl::transformPointCloud(src_dotted, src_dotted, affineRigidTransformation(src_dotted.makeShared(), tgt_dotted.makeShared()));
//
//	// result visualizer
//	pcl::visualization::PCLVisualizer::Ptr v(new pcl::visualization::PCLVisualizer);
//
//	// showing target mesh
//	v->addPolygonMesh(*tgt);
//	// showing "shouldbeonthesameplacethanthetarget" source
//	v->addPointCloud<pcl::PointXYZ>(src_dotted.makeShared(), "src_dotted");
//
//	v->spin();
//}
//
//void cancelPicks(void* args) {
//	_args* data = (_args*)args;
//	PointCloud::Ptr tmpCloud(new PointCloud);
//
//	// Clear picking point cloud
//	for (int i = 0; i < data->clicked_points_3d->size(); i++) {
//		data->viewerPtr->removePointCloud(data->id + i);
//		data->viewerPtr->addPointCloud(tmpCloud, data->id + i);
//	}
//	data->clicked_points_3d->clear();
//}
//
//#include <pcl/common/transforms.h>
//void handleKeyboardEvents(const pcl::visualization::KeyboardEvent &event, void* args){
//	if(event.keyDown())
//		switch (event.getKeyCode()) {
//		case 'e':
//			std::cout << "Bleeeeeh estimate" << std::endl;
//			// not anough point picked
//			if ((src_args.clicked_points_3d->size() < 4) || (tgt_args.clicked_points_3d->size() < 4))
//				break;
//			// Get rigid transformation estimation and display result
//			displayResult(getRigidTransformation(src_args.clicked_points_3d, tgt_args.clicked_points_3d));
//			break;
//		case 'r':
//			std::cout << "Bleeeeeh cancel" << std::endl;
//			cancelPicks(args);
//			break;
//		default:
//			break;
//		}
//}
//
//void initPickingClouds()
//{
//	// ----- start color stuff -----
//	// src
//	pcl::visualization::PointCloudColorHandlerCustom<pcl::PointXYZ> src_light_red(src_args.clicked_points_3d, 210, 50, 50); // light red
//	// tgt
//	pcl::visualization::PointCloudColorHandlerCustom<pcl::PointXYZ> tgt_light_red(tgt_args.clicked_points_3d, 210, 50, 50); // light red
//	// ----- end color stuff -----
//	// ----- igit -----
//	src_args.viewerPtr->addPointCloud(src_args.clicked_points_3d, src_light_red, "src_pick0");
//	src_args.viewerPtr->addPointCloud(src_args.clicked_points_3d, src_light_red, "src_pick1");
//	src_args.viewerPtr->addPointCloud(src_args.clicked_points_3d, src_light_red, "src_pick2");
//	src_args.viewerPtr->addPointCloud(src_args.clicked_points_3d, src_light_red, "src_pick3");
//	src_args.viewerPtr->addPointCloud(src_args.clicked_points_3d, src_light_red, "src_pick4");
//	src_args.viewerPtr->addPointCloud(src_args.clicked_points_3d, src_light_red, "src_pick5");
//	tgt_args.viewerPtr->addPointCloud(tgt_args.clicked_points_3d, tgt_light_red, "tgt_pick0");
//	tgt_args.viewerPtr->addPointCloud(tgt_args.clicked_points_3d, tgt_light_red, "tgt_pick1");
//	tgt_args.viewerPtr->addPointCloud(tgt_args.clicked_points_3d, tgt_light_red, "tgt_pick2");
//	tgt_args.viewerPtr->addPointCloud(tgt_args.clicked_points_3d, tgt_light_red, "tgt_pick3");
//	tgt_args.viewerPtr->addPointCloud(tgt_args.clicked_points_3d, tgt_light_red, "tgt_pick4");
//	tgt_args.viewerPtr->addPointCloud(tgt_args.clicked_points_3d, tgt_light_red, "tgt_pick5");
//}
//
//int main(int argc, char** argv)
//{
//	const char* f1 = "test/plan1-removed-duplicated-closemerged.ply";
//	const char* f2 = "test/plan2-removed-duplicated-closemerged.ply";
//
//	pcl::console::print_highlight("----- Loading1 -----\n");
//	IOPLY::load(f1, src);
//	pcl::console::print_highlight("----- Loading2 -----\n");
//	IOPLY::load(f2, tgt);
//
//	pcl::console::print_highlight("----- Loaded -----\n");
//
//	pcl::PointCloud<pcl::PointXYZ>::Ptr src_pick(new pcl::PointCloud<pcl::PointXYZ>);
//	pcl::PointCloud<pcl::PointXYZ>::Ptr tgt_pick(new pcl::PointCloud<pcl::PointXYZ>);
//
//	pcl::visualization::PCLVisualizer::Ptr viewer1(new pcl::visualization::PCLVisualizer);
//	pcl::visualization::PCLVisualizer::Ptr viewer2(new pcl::visualization::PCLVisualizer);
//
//	viewer1->addPolygonMesh(*src, "src");
//	viewer2->addPolygonMesh(*tgt, "tgt");
//
//	// ----- init callback args -----
//	src_args.id = "src_pick";
//	src_args.clicked_points_3d = src_pick;
//	src_args.viewerPtr = viewer1;
//	tgt_args.id = "tgt_pick";
//	tgt_args.clicked_points_3d = tgt_pick;
//	tgt_args.viewerPtr = viewer2;
//
//	// ----- init picking clouds -----
//	initPickingClouds();
//
//	viewer1->registerPointPickingCallback(pointPickingEventOccurred, (void*)&src_args);
//	viewer2->registerPointPickingCallback(pointPickingEventOccurred, (void*)&tgt_args);
//
//	viewer1->registerKeyboardCallback(handleKeyboardEvents, (void*)&src_args);
//	viewer2->registerKeyboardCallback(handleKeyboardEvents, (void*)&tgt_args);
//
//	viewer1->spin();
//	viewer2->spin();
//}