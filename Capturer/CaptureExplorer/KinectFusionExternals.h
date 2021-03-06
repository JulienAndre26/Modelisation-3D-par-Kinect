#pragma once

#include "stdafx.h"

#include <iostream>
#include <fstream>
#include <string>
#include <codecvt>
#include <stdarg.h>
#include <vector>

#include "XML.h"
#include "Standardizer.h"

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

	//static bool createMatrixFile(string path, int fileID, KinectFusionProcessor * processor) {
	//	string filename = path + string("/") + string("matrix_") + std::to_string(fileID) + string(".xml");
	//	wchar_t buffer[100];
	//	swprintf(buffer, 100, L"%s", filename);
	//	// remove already existing file
	//	if (fileExists(buffer)) {
	//		DeleteFile(buffer);
	//	}
	//	Matrix4 m = processor->getMatrix();
	//	XML  * xml = XML::Instance();
	//	xml->init();
	//	// first row
	//	xml->add(std::string("m11"), m.M11);
	//	xml->add(std::string("m12"), m.M12);
	//	xml->add(std::string("m13"), m.M13);
	//	xml->add(std::string("m14"), m.M14);
	//	// second row
	//	xml->add(std::string("m21"), m.M21);
	//	xml->add(std::string("m22"), m.M22);
	//	xml->add(std::string("m23"), m.M23);
	//	xml->add(std::string("m24"), m.M24);
	//	// third row
	//	xml->add(std::string("m31"), m.M31);
	//	xml->add(std::string("m32"), m.M32);
	//	xml->add(std::string("m33"), m.M33);
	//	xml->add(std::string("m34"), m.M34);
	//	// fourth row
	//	xml->add(std::string("m41"), m.M41);
	//	xml->add(std::string("m42"), m.M42);
	//	xml->add(std::string("m43"), m.M43);
	//	xml->add(std::string("m44"), m.M44);
	//	
	//	// true on success, false otherwise
	//	return xml->save(filename);
	//}
	
	static bool createConfFile(std::wstring wsConfPath, CREATE_CONF_EXT * sConf) {
		if (sConf == nullptr)
			return false;

		// Remove conf file if already exists
		if (fileExists(wsConfPath)) {
			LPOLESTR m_lFile = (LPOLESTR) new wchar_t[wsConfPath.length() + 1];
			std::wcscpy(m_lFile, wsConfPath.c_str());
			DeleteFile(m_lFile);
		}

		XML  * xml = XML::Instance(); 
		xml->init();
		xml->add(std::string("path"), Convert::toString(*(sConf->wsPath)));
		xml->add(std::string("count"), Convert::toString(*(sConf->wsCount)));
		//xml->add(std::string("format"), Convert::toString(*(sConf->wsFormat)));
		xml->add(std::string("color"), Convert::toString(*(sConf->wsColor)));
		xml->add(std::string("vpm"), Convert::toString(*(sConf->wsVPM)));
		xml->add(std::string("x"), Convert::toString(*(sConf->wsX)));
		xml->add(std::string("y"), Convert::toString(*(sConf->wsY)));
		xml->add(std::string("z"), Convert::toString(*(sConf->wsZ)));
		// true on success, false otherwise
		return xml->save(Convert::toString(wsConfPath));

	}

	template<typename ...Args>
	static bool check(Args... values) {
		return internalCheck(values..., "EOP");
	}

	template<typename ...Args>
	static bool internalCheck(const Args&... variables) {
		std::vector<string> vec = { variables... };
		bool isEverythingOK = true;
		string element;
		for (unsigned i = 0; i < vec.size(); ++i) {
			element = vec[i];
			if (element.compare("EOP") == 0) break;
			if (element.compare("ERROR") == 0) {
				isEverythingOK = false;
				break;
			}
		}
		return isEverythingOK;
	}

	static bool fileExists(std::wstring wsPath) {
		std::wifstream f(wsPath.c_str());
		bool bExists = f.good();
		f.close();
		return bExists;
	}

	static bool readConfFile(std::wstring wsConfPath, READ_CONF_EXT * sConf) {

		if (sConf == nullptr)
			return false;

		if (!fileExists(wsConfPath))
			return false;

		bool res = true;
		// Load mode
		XML * xml = XML::Instance();
		xml->init(Convert::toString(wsConfPath));
		// retrieve elements
		std::string path = xml->get("path");
		std::string count = xml->get("count");
		//std::string format = xml->get("format");
		std::string color = xml->get("color");
		std::string vpm = xml->get("vpm");
		std::string x = xml->get("x");
		std::string y = xml->get("y");
		std::string z = xml->get("z");
		// check for variables
		res = check(path, count, color, vpm, x, y, z);
		if (res) 
		{
			// add to structure
			sConf->nVPM = stoi(vpm);
			sConf->nX = stoi(x);
			sConf->nY = stoi(y);
			sConf->nZ = stoi(z);
			sConf->nMeshCount = stoi(count);
			sConf->bColor = color.compare("true") == 0;
			//sConf->nFormat = stoi(format);
		}
		
		// everything is ok
		return res;
	}
};

