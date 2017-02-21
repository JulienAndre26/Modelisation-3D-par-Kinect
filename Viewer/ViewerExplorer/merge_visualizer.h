#pragma once

#include "custom_typedef.h"
#include <IOPLY.h>
#include <stdio.h>
#include <string.h>

typedef struct {
	// structure used to pass arguments to the callback function
	const char* id;
	PointCloud::Ptr clicked_points_3d;
	Visualizer::Ptr viewerPtr;
	// MainWindow * ui;
} _merge_args;

class merge_visualizer
{
public:
	merge_visualizer() {}
	~merge_visualizer() {}

private:
	_merge_args src_args;
	_merge_args tgt_args;

};

