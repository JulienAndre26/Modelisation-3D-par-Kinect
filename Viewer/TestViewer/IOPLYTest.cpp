//#include "stdafx.h"
#include "CppUnitTest.h"

#include "IOPLY.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TestViewer
{		
	TEST_CLASS(IOPLYTest)
	{
	public:
		
		TEST_METHOD(LoadOK)
		{
			pcl::PolygonMesh::Ptr mesh(new PolygonMesh());
			//Assert::IsTrue(IOPLY::load("../../../TestViewer/toTest.ply", mesh));
			//Assert::IsTrue(mesh->polygons.size() > 0);
		}

		TEST_METHOD(LoadKO)
		{
			pcl::PolygonMesh::Ptr mesh(new PolygonMesh());
			Assert::IsFalse(IOPLY::load("notexist.ply", mesh));
		}
	};
}