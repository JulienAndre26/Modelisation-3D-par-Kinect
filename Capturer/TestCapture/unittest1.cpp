#include "stdafx.h"
#include "CppUnitTest.h"
#include "../CaptureExplorer/KinectFusionExternals.h"

#include <iostream>
#include <string>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TestCapture
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
			Assert::IsTrue(KinectFusionExternals::fileExists(L"./unittest1.cpp"));
			Assert::IsFalse(KinectFusionExternals::fileExists(L"./unittest1000.cpp"));
		}

	};
}