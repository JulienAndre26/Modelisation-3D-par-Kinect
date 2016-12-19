#include "KinectFusionExternals.h"

bool KinectFusionExternals::createConfFile(std::wstring wsConfPath, CONF_EXT * sConf)
{
	bool bRes = true;

	bool bExists = fileExists(wsConfPath);

	// Remove conf file if already exists
	if (bExists) {
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
	}
	else {
		bRes = false;
	}

	return bRes;
}

bool KinectFusionExternals::fileExists(std::wstring wsPath)
{
	std::wifstream f(wsPath.c_str());
	bool bExists = f.good();
	f.close();
	return bExists;
}

