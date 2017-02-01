//#include "stdafx.h"
#include "CppUnitTest.h"

#include "IOPLY.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TestViewer
{		
	TEST_CLASS(IOPLYTest)
	{
	public:
		/* LOAD */

		TEST_METHOD(LoadPolygonMeshOK)
		{
			pcl::PolygonMesh::Ptr mesh(new PolygonMesh());
			
			Assert::IsFalse(mesh->cloud.height > 0);
			Assert::IsFalse(mesh->cloud.width > 0);

			/*Assert::IsTrue(IOPLY::load("../../../TestViewer/toTest.ply", mesh));

			Assert::IsTrue(mesh->cloud.height > 0);
			Assert::IsTrue(mesh->cloud.width > 0);*/
		}

		TEST_METHOD(LoadPointCloudOK)
		{
			PointCloud::Ptr cloud(new PointCloud());
			//Assert::IsTrue(IOPLY::load("../../../TestViewer/toTest.ply", cloud));
			//Assert::IsTrue(cloud->points.size() > 0);
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