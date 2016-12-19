#include "stdafx.h"
#include "CppUnitTest.h"
#include "../ViewerExplorer/headers/Processor/Processor.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TestViewer
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
			Assert::AreEqual(1, Processor::computeMetrics(1, 1, 1, 1, 1, 1).returnedDouble, 1);
			
			// TODO: Your test code here
		}

	};
}