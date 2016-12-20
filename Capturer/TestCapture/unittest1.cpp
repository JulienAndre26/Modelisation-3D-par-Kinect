#include "stdafx.h"
#include "CppUnitTest.h"

#include "../CaptureExplorer/KinectFusionExternals.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TestCapture
{		
	TEST_CLASS(UnitTest1)
	{

	private:
		CREATE_CONF_EXT * sConf;
		void init_create() {
			sConf = (CREATE_CONF_EXT *)malloc(sizeof(CREATE_CONF_EXT));
			std::wstring wsVPM = L"wsVPM";
			sConf->wsVPM = &wsVPM;
			std::wstring wsX = L"wsX";
			sConf->wsX = &wsX;
			std::wstring wsY = L"wsY";
			sConf->wsY = &wsY;
			std::wstring wsZ = L"wsZ";
			sConf->wsZ = &wsZ;
		}

	public:
				
		TEST_METHOD(FileExistsFalse)
		{
			Assert::IsFalse(KinectFusionExternals::fileExists(L"./woua"));
		}

		TEST_METHOD(FileExistsTrue)
		{
			std::wstring wsPath(L"./test.txt");

			// Create file
			std::wofstream confFile(wsPath, std::ios::out | std::ios::trunc);

			Assert::IsTrue(KinectFusionExternals::fileExists(wsPath));

			DeleteFile(wsPath.c_str());
		}

		TEST_METHOD(CreateImportFileTrue)
		{
			init_create();

		}
	};			

}