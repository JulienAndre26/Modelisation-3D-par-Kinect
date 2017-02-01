//#include "stdafx.h"
#include "CppUnitTest.h"

#include "IIO.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TestViewer
{		
	TEST_CLASS(IIOTest)
	{
	public:
		
		TEST_METHOD(FileExistsOK)
		{
			Assert::IsTrue(IIO::fileExists(std::string("../../../TestViewer/toTest.ply")));
		}

		TEST_METHOD(FileExistsKO)
		{
			Assert::IsFalse(IIO::fileExists(std::string("notexist.ply")));
		}
	};
}