//#include "stdafx.h"
#include "CppUnitTest.h"

#include "IOUtils.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TestViewer
{		
	TEST_CLASS(IOUtilsTest)
	{
	public:
		
		TEST_METHOD(FileExistsOK)
		{
			Assert::IsTrue(IOUtils::fileExists(std::string("../../../TestViewer/toTest.ply")));
		}

		TEST_METHOD(FileExistsKO)
		{
			Assert::IsFalse(IOUtils::fileExists(std::string("notexist.ply")));
		}
	};
}