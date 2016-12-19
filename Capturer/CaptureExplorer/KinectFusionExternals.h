#pragma once

#include "stdafx.h"

#include <iostream>
#include <fstream>

typedef struct {
	std::wstring * wsVPM;
	std::wstring * wsX;
	std::wstring * wsY;
	std::wstring * wsZ;
	std::wstring * wsFormat;
	std::wstring * wsColor;
	std::wstring * wsCount;
	std::wstring * wsPath;
} CONF_EXT;

class KinectFusionExternals
{
public:
	KinectFusionExternals() {};
	~KinectFusionExternals() {};
	bool createConfFile(std::wstring wsPath, CONF_EXT * sConf);
	bool fileExists(std::wstring wsPath);
};

