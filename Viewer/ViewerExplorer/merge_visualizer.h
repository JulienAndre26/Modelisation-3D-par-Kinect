#pragma once

#include "custom_typedef.h"
#include <IOPLY.h>
#include <PCLCore.h>
#include <stdio.h>
#include <string.h>

class MergeVisualizer;

typedef struct {
	// structure used to pass arguments to the callback function
	const char* id;
	PointCloud::Ptr clicked_points_3d;
	Visualizer::Ptr viewerPtr;
	// MainWindow * ui;
	MergeVisualizer* daddy;
} _merge_args;

class MergeVisualizer
{
public:
	// Max number of picking point
	static const int MAX_PICKING_POINTS = 6;
	// Min number of picking point
	static const int MIN_PICKING_POINTS = 4;

	MergeVisualizer(const std::string& from, const std::string& to, const std::string& into) {
		finished = false;
		m_saveTarget = into;

		// loading special stuff for merge visualizer
		loadMeshes(from, to);
		initCallbackArgs();
		initVisualizer();
	}

	// display merge result
	void displayResult();

	bool hasEnoughPickingPoints(){
		return ((m_src_args.clicked_points_3d->size() >= MIN_PICKING_POINTS) &&
			(m_tgt_args.clicked_points_3d->size() >= MIN_PICKING_POINTS));
	}

	~MergeVisualizer() {}
private:
	// save target path
	std::string m_saveTarget;
	// callback structs for both visualizers
	_merge_args m_src_args;
	_merge_args m_tgt_args;
	// Source meshes
	PolygonMesh::Ptr m_src;
	PolygonMesh::Ptr m_tgt;
	// Merge processing
	bool finished;

	// load meshes to merge
	void loadMeshes(const std::string& from, const std::string& to);
	// init callback args for each visualizers
	void initCallbackArgs();
	// init both visualizers for point picking in order to estimate a rigid transformation to merge both meshes
	void initVisualizer();
};

