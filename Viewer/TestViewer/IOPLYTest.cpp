//#include "stdafx.h"
#include "CppUnitTest.h"

#include "IOPLY.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TestViewer
{		
	TEST_CLASS(IOPLYTest)
	{
	public:
		
		TEST_METHOD(LoadPolygonMeshOK)
		{
			pcl::PolygonMesh::Ptr mesh(new PolygonMesh());
			//Assert::IsTrue(IOPLY::load("../../../TestViewer/toTest.ply", mesh));
			//Assert::IsTrue(mesh->polygons.size() > 0);
		}

		TEST_METHOD(LoadPointCloudOK)
		{
			PointCloud::Ptr mesh(new PointCloud());
			//Assert::IsTrue(IOPLY::load("../../../TestViewer/toTest.ply", mesh));
		}

		TEST_METHOD(LoadPointCloudColoredOK)
		{
			PointCloudColored::Ptr mesh(new PointCloudColored());
			//Assert::IsTrue(IOPLY::load("../../../TestViewer/toTest.ply", mesh));
		}

		TEST_METHOD(LoadPointCloudNormalsOK)
		{
			PointCloudNormals::Ptr mesh(new PointCloudNormals());
			//Assert::IsTrue(IOPLY::load("../../../TestViewer/toTest.ply", mesh));
		}

		TEST_METHOD(LoadPointCloudFPFHOK)
		{
			PointCloudFPFH::Ptr mesh(new PointCloudFPFH());
			//Assert::IsTrue(IOPLY::load("../../../TestViewer/toTest.ply", mesh));
		}

		TEST_METHOD(LoadPolygonMeshKO)
		{
			pcl::PolygonMesh::Ptr mesh(new PolygonMesh());
			Assert::IsFalse(IOPLY::load("notexist.ply", mesh));
		}

		TEST_METHOD(LoadPointCloudKO)
		{
			PointCloud::Ptr mesh(new PointCloud());
			Assert::IsFalse(IOPLY::load("notexist.ply", mesh));
		}

		TEST_METHOD(LoadPointCloudColoredKO)
		{
			PointCloudColored::Ptr mesh(new PointCloudColored());
			Assert::IsFalse(IOPLY::load("notexist.ply", mesh));
		}

		TEST_METHOD(LoadPointCloudNormalsKO)
		{
			PointCloudNormals::Ptr mesh(new PointCloudNormals());
			Assert::IsFalse(IOPLY::load("notexist.ply", mesh));
		}

		TEST_METHOD(LoadPointCloudFPFHKO)
		{
			PointCloudFPFH::Ptr mesh(new PointCloudFPFH());
			Assert::IsFalse(IOPLY::load("notexist.ply", mesh));
		}
	};
}