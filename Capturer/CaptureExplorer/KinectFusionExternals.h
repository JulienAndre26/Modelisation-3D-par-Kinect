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
	
	static bool createConfFile(std::wstring wsConfPath, CREATE_CONF_EXT * sConf) {
		if (sConf == nullptr)
			return false;

		// Remove conf file if already exists
		if (fileExists(wsConfPath)) {
			LPOLESTR m_lFile = (LPOLESTR) new wchar_t[wsConfPath.length() + 1];
			std::wcscpy(m_lFile, wsConfPath.c_str());
			DeleteFile(m_lFile);
		}

		std::wofstream confFile(wsConfPath, std::ios::out | std::ios::trunc);  //déclaration du flux et ouverture du fichier

		// If success -> write
		if (confFile)
		{
			confFile << L"[PATH] " + *(sConf->wsPath) + L"\n";
			confFile << L"[COUNT] " + *(sConf->wsCount) + L"\n";
			confFile << L"[FORMAT] " + *(sConf->wsFormat) + L"\n";
			confFile << L"[COLOR] " + *(sConf->wsColor) + L"\n";
			confFile << L"[VPM] " + *(sConf->wsVPM) + L"\n";
			confFile << L"[X] " + *(sConf->wsX) + L"\n";
			confFile << L"[Y] " + *(sConf->wsY) + L"\n";
			confFile << L"[Z] " + *(sConf->wsZ);

			confFile.close();
			return true;
		}
		
		return false;
	}

	static bool fileExists(std::wstring wsPath) {
		std::wifstream f(wsPath.c_str());
		bool bExists = f.good();
		f.close();
		return bExists;
	}

	static bool readConfFile(std::wstring wsConfPath, READ_CONF_EXT * sConf) {
		std::wifstream confFile(wsConfPath);

		if (!confFile.is_open() || sConf == nullptr)
			return false;

		// Prepare converter UTF8
		const std::locale empty_locale = std::locale::empty();
		typedef std::codecvt_utf8<wchar_t> converter_type;
		const converter_type* converter = new converter_type;
		const std::locale utf8_locale = std::locale(empty_locale, converter);
		confFile.imbue(utf8_locale);

		std::wstring wsLine;
		size_t nSpaceIndex;
		bool res = true;

		// PATH
		if (std::getline(confFile, wsLine) && res)
		{
			nSpaceIndex = wsLine.find_first_of(L" ");
			wsLine = wsLine.substr(nSpaceIndex + 1, wsLine.length());
		}
		else
			res = false;

		// COUNT
		int nMeshCount = -1;
		if (std::getline(confFile, wsLine) && res)
		{
			nSpaceIndex = wsLine.find_first_of(L" ");
			wsLine = wsLine.substr(nSpaceIndex + 1, wsLine.length());

			nMeshCount = stoi(wsLine);
		}
		else
			res = false;

		// FORMAT
		int nFormat;
		if (std::getline(confFile, wsLine) && res)
		{
			nSpaceIndex = wsLine.find_first_of(L" ");
			wsLine = wsLine.substr(nSpaceIndex + 1, wsLine.length());

			if (wsLine.compare(L"ply") == 0)
				nFormat = PLY;
			else if (wsLine.compare(L"obj") == 0)
				nFormat = OBJ;
			else if (wsLine.compare(L"stl") == 0)
				nFormat = STL;
			else
				res = false;
		}
		else
			res = false;

		// COLOR
		bool bColor = false;
		if (std::getline(confFile, wsLine) && res)
		{
			nSpaceIndex = wsLine.find_first_of(L" ");
			wsLine = wsLine.substr(nSpaceIndex + 1, wsLine.length());

			if (wsLine.compare(L"true") == 0)
				bColor = true;
			else if (wsLine.compare(L"false") == 0)
				bColor = false;
			else
				res = false;
		}
		else
			res = false;

		// QUALITY
		int nVPM = -1;
		int nX = -1;
		int nY = -1;
		int nZ = -1;

		// VPM
		if (std::getline(confFile, wsLine) && res)
		{
			nSpaceIndex = wsLine.find_first_of(L" ");
			wsLine = wsLine.substr(nSpaceIndex + 1, wsLine.length());

			nVPM = std::stoi(wsLine);
		}
		else
			res = false;

		// X
		if (std::getline(confFile, wsLine) && res)
		{
			nSpaceIndex = wsLine.find_first_of(L" ");
			wsLine = wsLine.substr(nSpaceIndex + 1, wsLine.length());

			nX = std::stoi(wsLine);
		}
		else
			res = false;

		// Y
		if (std::getline(confFile, wsLine) && res)
		{
			nSpaceIndex = wsLine.find_first_of(L" ");
			wsLine = wsLine.substr(nSpaceIndex + 1, wsLine.length());

			nY = std::stoi(wsLine);
		}
		else
			res = false;

		// Z
		if (std::getline(confFile, wsLine) && res)
		{
			nSpaceIndex = wsLine.find_first_of(L" ");
			wsLine = wsLine.substr(nSpaceIndex + 1, wsLine.length());

			nZ = std::stoi(wsLine);
		}
		else
			res = false;

		if (res)
		{
			sConf->nMeshCount = nMeshCount;
			sConf->nFormat = nFormat;
			sConf->bColor = bColor;
			sConf->nVPM = nVPM;
			sConf->nX = nX;
			sConf->nY = nY;
			sConf->nZ = nZ;
		}

		confFile.close();

		return res;
	}
};

