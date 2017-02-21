#include "merge_visualizer.h"

void MergeVisualizer::loadMeshes(const std::string& from, const std::string& to) {
	IOPLY::load(from.c_str(), m_src);
	IOPLY::load(to.c_str(), m_tgt);
}

void MergeVisualizer::initCallbackArgs() {
	// keys to identify picked points
	m_src_args.id = "src_pick";
	m_tgt_args.id = "tgt_pick";
	// objects to visualize picking points
	m_src_args.viewerPtr = Visualizer::Ptr(new Visualizer);
	m_tgt_args.viewerPtr = Visualizer::Ptr(new Visualizer);
	// objects to store picked points
	m_src_args.clicked_points_3d = PointCloud::Ptr(new PointCloud);
	m_tgt_args.clicked_points_3d = PointCloud::Ptr(new PointCloud);
	// reference to daddy(this) for specific functions called in event handlers
	m_src_args.daddy = this;
	m_tgt_args.daddy = this;
}


// ----- Point picking events handler ------
void pointPickingEventOccurred(const pcl::visualization::PointPickingEvent &event, void* args)
{
	_merge_args* data = (_merge_args*)args;

	// Max elem reached, don't add any other element
	if (data->clicked_points_3d->size() >= MergeVisualizer::MAX_PICKING_POINTS)
		return;

	// Get clicked 3D point from picking event
	PointT point;
	event.getPoint(point.x, point.y, point.z);

	// @debug pcl::console::print_info("Added point : {x: %f, y: %f, z: %f}\n", point.x, point.y, point.z);

	// Magic trick to add only different points
	for (int i = 0; i < data->clicked_points_3d->size(); i++)
		if ((data->clicked_points_3d->at(i).x == point.x) &&
			(data->clicked_points_3d->at(i).y == point.y) &&
			(data->clicked_points_3d->at(i).z == point.z))
			return;

	// push back clicked 3D point
	data->clicked_points_3d->push_back(point);

	bool baseColor;
	PointCloud::Ptr tmpCloud(new PointCloud);
	// Update each colored point
	for (int i = 0; i < data->clicked_points_3d->size(); i++) {
		data->viewerPtr->removePointCloud(data->id + i);
		baseColor = (i / 3 == 0);
		// add current point to tmp cloud
		tmpCloud->push_back(data->clicked_points_3d->at(i));
		// display points with unique color (6 colors max)
		data->viewerPtr->addPointCloud(tmpCloud, pcl::visualization::PointCloudColorHandlerCustom<PointT>(data->clicked_points_3d,
			((i % 3) == 0) ? (baseColor ? 210 : 50) : (baseColor ? 50 : 210),
			((i % 3) == 1) ? (baseColor ? 210 : 50) : (baseColor ? 50 : 210),
			((i % 3) == 2) ? (baseColor ? 210 : 50) : (baseColor ? 50 : 210)), data->id + i);
		// big point
		data->viewerPtr->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 13, data->id + i);
		// reset point cloud
		tmpCloud->clear();
	}
}

// ----- keyboard events -----
void MergeVisualizer::displayResult() {
	PointCloud::Ptr src_dotted;
	// transform point cloud
	PCLCore::transformMesh(m_src, src_dotted, m_src_args.clicked_points_3d, m_tgt_args.clicked_points_3d);
	// close viewers for point picking
	m_src_args.viewerPtr->close();
	m_tgt_args.viewerPtr->close();

	// result visualizer
	Visualizer::Ptr v(new Visualizer);

	// showing target mesh
	v->addPolygonMesh(*m_tgt);
	// Adding empty points clouds to each viewer
	// showing "shouldbeonthesameplacethanthetarget" source
	v->addPointCloud(src_dotted, "src_dotted");

	v->spin();
}

void cancelPicks(void* args) {
	_merge_args* data = (_merge_args*)args;

	PointCloud::Ptr tmpCloud(new PointCloud);
	// Clear picking point cloud
	for (int i = 0; i < data->clicked_points_3d->size(); i++) {
		data->viewerPtr->removePointCloud(data->id + i);
		// Needed to avoid errors from remove point cloud
		data->viewerPtr->addPointCloud(tmpCloud, data->id + i);
	}
	data->clicked_points_3d->clear();
}

// ----- Keyboard events handler -----
void handleKeyboardEvents(const pcl::visualization::KeyboardEvent &event, void* args) {
	_merge_args* data;
	if (event.keyDown())
		switch (event.getKeyCode()) {
		case 'e':
			data = (_merge_args*)args;
			// not anough point picked
			if (!data->daddy->hasEnoughPickingPoints())
				break;
			data->daddy->displayResult();
			break;
		case 'r':
			cancelPicks(args);
			break;
		default:
			break;
		}
}

void MergeVisualizer::initVisualizer() {
	// view meshes to pick points
	m_src_args.viewerPtr->addPolygonMesh(*m_src, "src");
	m_src_args.viewerPtr->addPolygonMesh(*m_src, "src");

	// Adding empty points clouds to each viewer
	// Needed to avoid errors from remove point cloud
	for (int i = 0; i < MergeVisualizer::MAX_PICKING_POINTS; i++) {
		m_src_args.viewerPtr->addPointCloud(m_src_args.clicked_points_3d, m_src_args.id + i);
		m_tgt_args.viewerPtr->addPointCloud(m_tgt_args.clicked_points_3d, m_tgt_args.id + i);
	}

	// register point picking event with associated callback args
	m_src_args.viewerPtr->registerPointPickingCallback(pointPickingEventOccurred, (void*)&m_src_args);
	m_tgt_args.viewerPtr->registerPointPickingCallback(pointPickingEventOccurred, (void*)&m_tgt_args);
	
	// register keyboard callback event with associated callback args
	m_src_args.viewerPtr->registerKeyboardCallback(handleKeyboardEvents, (void*)&m_src_args);
	m_tgt_args.viewerPtr->registerKeyboardCallback(handleKeyboardEvents, (void*)&m_tgt_args);
	
	// display viewers
	m_src_args.viewerPtr->spin();
	m_tgt_args.viewerPtr->spin();
}
