//#include "stdafx.h"
#include "CppUnitTest.h"

#include "../CaptureExplorer/KinectFusionExternals.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TestCapture
{		
	TEST_CLASS(UnitTest1)
	{

	private:
		// For Create
		std::wstring wsVPM;
		std::wstring wsX;
		std::wstring wsY;
		std::wstring wsZ;
		std::wstring wsFormat;
		std::wstring wsColor;
		std::wstring wsCount;
		std::wstring wsPath;
				
		void init_create(CREATE_CONF_EXT * sConf) 
		{	
			wsVPM = L"123456.0";
			sConf->wsVPM = &wsVPM;
			wsX = L"12";
			sConf->wsX = &wsX;
			wsY = L"34";
			sConf->wsY = &wsY;
			wsZ = L"56";
			sConf->wsZ = &wsZ;
			wsFormat = L"ply";
			sConf->wsFormat = &wsFormat;
			wsColor = L"true";
			sConf->wsColor = &wsColor;
			wsCount = std::to_wstring(1);
			sConf->wsCount = &wsCount;
			wsPath = L".";
			sConf->wsPath = &wsPath;
		}

		void removeLineFromFile(std::wstring wsSrc, std::wstring wsDst, int nLineNumber)
		{
			std::wifstream src(wsSrc);

			if (!src.is_open())
				return;

			// Prepare converter UTF8
			const std::locale empty_locale = std::locale::empty();
			typedef std::codecvt_utf8<wchar_t> converter_type;
			const converter_type* converter = new converter_type;
			const std::locale utf8_locale = std::locale(empty_locale, converter);
			src.imbue(utf8_locale);

			std::wofstream dst(wsDst, std::ios::out | std::ios::trunc);  //déclaration du flux et ouverture du fichier

			if (dst)
			{
				int n = 1;
				std::wstring wsLine;
				while (getline(src, wsLine))
				{
					if (n != nLineNumber)
						dst << wsLine << "\n";
					n++;
				}

				dst.close();
			}

			src.close();
		}

	public:
				
		TEST_METHOD(FileExistsFalse)
		{
			Assert::IsFalse(KinectFusionExternals::fileExists(L"./woua"));
		}

		TEST_METHOD(FileExistsTrue)
		{
			std::wstring wsPath(L"./test.txt");
			std::wofstream confFile(wsPath, std::ios::out | std::ios::trunc);

			Assert::IsTrue(KinectFusionExternals::fileExists(wsPath));

			DeleteFile(wsPath.c_str());
		}

		TEST_METHOD(CreateImportFileOK)
		{
			CREATE_CONF_EXT * sConf = (CREATE_CONF_EXT *)malloc(sizeof(CREATE_CONF_EXT));
			
			init_create(sConf);
			std::wstring wsConfPath(L"./test.import");

			Assert::IsTrue(KinectFusionExternals::createConfFile(wsConfPath, sConf));

			DeleteFile(wsConfPath.c_str());
			free(sConf);
		}

		TEST_METHOD(CreateImportFileKO)
		{
			Assert::IsFalse(KinectFusionExternals::createConfFile(L".", nullptr));
		}

		TEST_METHOD(ReadImportFileOK)
		{
			CREATE_CONF_EXT * sConfW = (CREATE_CONF_EXT *)malloc(sizeof(CREATE_CONF_EXT));

			init_create(sConfW);
			std::wstring wsConfPath(L"./test.import");

			Assert::IsTrue(KinectFusionExternals::createConfFile(wsConfPath, sConfW));

			READ_CONF_EXT * sConfR = (READ_CONF_EXT *)malloc(sizeof(READ_CONF_EXT));

			Assert::IsTrue(KinectFusionExternals::readConfFile(wsConfPath, sConfR));

			DeleteFile(wsConfPath.c_str());
			free(sConfR);
			free(sConfW);
		}

		TEST_METHOD(ReadImportFileKO1)
		{
			CREATE_CONF_EXT * sConfW = (CREATE_CONF_EXT *)malloc(sizeof(CREATE_CONF_EXT));

			init_create(sConfW);
			std::wstring wsConfPath(L"./test.import");

			Assert::IsTrue(KinectFusionExternals::createConfFile(wsConfPath, sConfW));

			Assert::IsFalse(KinectFusionExternals::readConfFile(wsConfPath, nullptr));

			DeleteFile(wsConfPath.c_str());
			free(sConfW);
		}

		TEST_METHOD(ReadImportFileKO2)
		{
			READ_CONF_EXT * sConfR = (READ_CONF_EXT *)malloc(sizeof(READ_CONF_EXT));

			Assert::IsFalse(KinectFusionExternals::readConfFile(L"./doesnotexist.prout", sConfR));
		}

		TEST_METHOD(ReadImportFileKO3)
		{
			CREATE_CONF_EXT * sConfW = (CREATE_CONF_EXT *)malloc(sizeof(CREATE_CONF_EXT));

			init_create(sConfW);
			std::wstring wsConfPath(L"./test.import");
			std::wstring wsConfEditPath(L"./testEdit.import");

			Assert::IsTrue(KinectFusionExternals::createConfFile(wsConfPath, sConfW));

			removeLineFromFile(wsConfPath, wsConfEditPath, 8); // No Z

			READ_CONF_EXT * sConfR = (READ_CONF_EXT *)malloc(sizeof(READ_CONF_EXT));

			Assert::IsFalse(KinectFusionExternals::readConfFile(wsConfEditPath, sConfR));

			DeleteFile(wsConfPath.c_str());
			DeleteFile(wsConfEditPath.c_str());
			free(sConfR);
			free(sConfW);
		}

		TEST_METHOD(ReadImportFileKO4)
		{
			CREATE_CONF_EXT * sConfW = (CREATE_CONF_EXT *)malloc(sizeof(CREATE_CONF_EXT));

			init_create(sConfW);
			std::wstring wsConfPath(L"./test.import");
			std::wstring wsConfEditPath(L"./testEdit.import");

			Assert::IsTrue(KinectFusionExternals::createConfFile(wsConfPath, sConfW));

			removeLineFromFile(wsConfPath, wsConfEditPath, 7); // No Y

			READ_CONF_EXT * sConfR = (READ_CONF_EXT *)malloc(sizeof(READ_CONF_EXT));

			Assert::IsFalse(KinectFusionExternals::readConfFile(wsConfEditPath, sConfR));

			DeleteFile(wsConfPath.c_str());
			DeleteFile(wsConfEditPath.c_str());
			free(sConfR);
			free(sConfW);
		}

		TEST_METHOD(ReadImportFileKO5)
		{
			CREATE_CONF_EXT * sConfW = (CREATE_CONF_EXT *)malloc(sizeof(CREATE_CONF_EXT));

			init_create(sConfW);
			std::wstring wsConfPath(L"./test.import");
			std::wstring wsConfEditPath(L"./testEdit.import");

			Assert::IsTrue(KinectFusionExternals::createConfFile(wsConfPath, sConfW));

			removeLineFromFile(wsConfPath, wsConfEditPath, 6); // No X

			READ_CONF_EXT * sConfR = (READ_CONF_EXT *)malloc(sizeof(READ_CONF_EXT));

			Assert::IsFalse(KinectFusionExternals::readConfFile(wsConfEditPath, sConfR));

			DeleteFile(wsConfPath.c_str());
			DeleteFile(wsConfEditPath.c_str());
			free(sConfR);
			free(sConfW);
		}

		TEST_METHOD(ReadImportFileKO6)
		{
			CREATE_CONF_EXT * sConfW = (CREATE_CONF_EXT *)malloc(sizeof(CREATE_CONF_EXT));

			init_create(sConfW);
			std::wstring wsConfPath(L"./test.import");
			std::wstring wsConfEditPath(L"./testEdit.import");

			Assert::IsTrue(KinectFusionExternals::createConfFile(wsConfPath, sConfW));

			removeLineFromFile(wsConfPath, wsConfEditPath, 5); // No VPM

			READ_CONF_EXT * sConfR = (READ_CONF_EXT *)malloc(sizeof(READ_CONF_EXT));

			Assert::IsFalse(KinectFusionExternals::readConfFile(wsConfEditPath, sConfR));

			DeleteFile(wsConfPath.c_str());
			DeleteFile(wsConfEditPath.c_str());
			free(sConfR);
			free(sConfW);
		}

		TEST_METHOD(CheckOK1)
		{
			Assert::IsTrue(KinectFusionExternals::check("EOP"));
		}

		TEST_METHOD(CheckOK2)
		{
			Assert::IsTrue(KinectFusionExternals::check("EOP", "ERROR"));
		}

		TEST_METHOD(CheckOK3)
		{
			Assert::IsTrue(KinectFusionExternals::check("NOT ERROR"));
		}

		TEST_METHOD(CheckOK4)
		{
			Assert::IsTrue(KinectFusionExternals::check(""));
		}

		TEST_METHOD(CheckKO1)
		{
			Assert::IsFalse(KinectFusionExternals::check("ERROR", "EOP"));
		}

		TEST_METHOD(CheckKO2)
		{
			Assert::IsFalse(KinectFusionExternals::check("ERROR"));
		}
	};

}