#pragma once

#include "stdafx.h"

#include <iostream>
#include <fstream>
#include <string>
#include <codecvt>

#define STL 0
#define OBJ 1
#define PLY 2

typedef struct {
	std::wstring * wsVPM;
	std::wstring * wsX;
	std::wstring * wsY;
	std::wstring * wsZ;
	std::wstring * wsFormat;
	std::wstring * wsColor;
	std::wstring * wsCount;
	std::wstring * wsPath;
} CREATE_CONF_EXT;

typedef struct {
	int nVPM;
	int nX;
	int nY;
	int nZ;
	int nMeshCount;
	bool bColor;
	int nFormat;
} READ_CONF_EXT;

class KinectFusionExternals
{
public:
	KinectFusionExternals() {};
	~KinectFusionExternals() {};
	bool createConfFile(std::wstring wsPath, CREATE_CONF_EXT * sConf);
	bool fileExists(std::wstring wsPath);
	bool readConfFile(std::wstring wsPath, READ_CONF_EXT * sConf);
};

