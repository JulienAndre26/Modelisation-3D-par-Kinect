//#include "stdafx.h"
#include "CppUnitTest.h"

#include "Processor.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TestViewer
{		
	TEST_CLASS(ProcessorTest)
	{
	public:
		
		TEST_METHOD(TestComputeMetricsOK1)
		{
			double expected = 0.0;
			double actual = Processor::computeMetrics(1.0, 1.0, 1.0, 1.0, 1.0, 1.0).returnedDouble;
			Assert::AreEqual(expected, actual);
		}

		TEST_METHOD(TestComputeMetricsOK2)
		{
			double expected = 1.0;
			double actual = Processor::computeMetrics(1.0, 0.0, 0.0, 0.0, 0.0, 0.0).returnedDouble;
			Assert::AreEqual(expected, actual);
		}

		TEST_METHOD(TestComputeMetricsOK3)
		{
			double expected = 1.0;
			double actual = Processor::computeMetrics(0.0, 1.0, 0.0, 0.0, 0.0, 0.0).returnedDouble;
			Assert::AreEqual(expected, actual);
		}

		TEST_METHOD(TestComputeMetricsOK4)
		{
			double expected = 1.0;
			double actual = Processor::computeMetrics(0.0, 0.0, 1.0, 0.0, 0.0, 0.0).returnedDouble;
			Assert::AreEqual(expected, actual);
		}

		TEST_METHOD(TestComputeMetricsOK5)
		{
			double expected = 1.0;
			double actual = Processor::computeMetrics(0.0, 0.0, 0.0, 1.0, 0.0, 0.0).returnedDouble;
			Assert::AreEqual(expected, actual);
		}

		TEST_METHOD(TestComputeMetricsOK6)
		{
			double expected = 1.0;
			double actual = Processor::computeMetrics(0.0, 0.0, 0.0, 0.0, 1.0, 0.0).returnedDouble;
			Assert::AreEqual(expected, actual);
		}

		TEST_METHOD(TestComputeMetricsOK7)
		{
			double expected = 1.0;
			double actual = Processor::computeMetrics(0.0, 0.0, 0.0, 0.0, 0.0, 1.0).returnedDouble;
			Assert::AreEqual(expected, actual);
		}

		TEST_METHOD(TestComputeMetricsKO)
		{
			double expected = 1.0;
			double actual = Processor::computeMetrics(0.0, 0.0, 0.0, 0.0, 0.0, 2.0).returnedDouble;
			Assert::AreNotEqual(expected, actual);
		}
	};
}