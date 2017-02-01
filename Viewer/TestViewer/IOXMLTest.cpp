//#include "stdafx.h"
#include "CppUnitTest.h"

#include "IOXML.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TestViewer
{		
	TEST_CLASS(IOXMLTest)
	{
	private:
		IOXML * xmlManager;

	public:
		/* GET INSTANCE */

		TEST_METHOD_INITIALIZE(GetInstance)
		{
			xmlManager = IOXML::Instance();
		}

		
		/* INIT */
		
		TEST_METHOD(InitOK)
		{
			Assert::AreEqual(xmlManager->init("../../../TestViewer/toTest.import"), XML_OK);
		}

		TEST_METHOD(InitKO1)
		{
			Assert::AreEqual(xmlManager->init(), XML_IMPORT_ERROR);
		}

		TEST_METHOD(InitKO2)
		{
			Assert::AreEqual(xmlManager->init("doesnotexist.import"), XML_IMPORT_ERROR);
		}

		
		/* GET STRING */
		
		TEST_METHOD(GetStringOK1)
		{
			Assert::AreEqual(xmlManager->init("../../../TestViewer/toTest.import"), XML_OK);
			std::string res = xmlManager->getString("color");
			Assert::IsTrue(res.compare("false") == 0);
		}

		TEST_METHOD(GetStringOK2)
		{
			Assert::AreEqual(xmlManager->init("../../../TestViewer/toTest.import"), XML_OK);
			std::string res = xmlManager->getString("count");
			Assert::IsTrue(res.compare("1") == 0);
		}

		TEST_METHOD(GetStringKO1)
		{
			Assert::AreEqual(xmlManager->init("../../../TestViewer/toTest.import"), XML_OK);
			std::string res = xmlManager->getString("doesnotexist");
			Assert::IsTrue(res.compare(XML_GET_STR_ERROR) == 0);
		}
		
		TEST_METHOD(GetStringKO2)
		{
			Assert::AreEqual(xmlManager->init("../../../TestViewer/toTest.import"), XML_OK);
			std::string res = xmlManager->getString("color");
			Assert::IsFalse(res.compare("true") == 0);
		}


		/* GET FLOAT */

		TEST_METHOD(GetFloatOK1)
		{
			Assert::AreEqual(xmlManager->init("../../../TestViewer/toTest.import"), XML_OK);
			Assert::AreEqual(xmlManager->getFloat("vpm"), (float) 32.0);
		}

		TEST_METHOD(GetFloatOK2)
		{
			Assert::AreEqual(xmlManager->init("../../../TestViewer/toTest.import"), XML_OK);
			Assert::AreEqual(xmlManager->getFloat("x"), (float) 320.0);
		}

		TEST_METHOD(GetFloatKO1)
		{
			Assert::AreEqual(xmlManager->init("../../../TestViewer/toTest.import"), XML_OK);
			Assert::AreEqual(xmlManager->getFloat("doesnotexist"), (float)XML_GET_FLOAT_ERROR);
		}

		TEST_METHOD(GetFloatKO2)
		{
			Assert::AreEqual(xmlManager->init("../../../TestViewer/toTest.import"), XML_OK);
			Assert::AreEqual(xmlManager->getFloat("path"), (float)XML_GET_FLOAT_ERROR);
		}


		/* GET INT */
		
		TEST_METHOD(GetIntOK1)
		{
			Assert::AreEqual(xmlManager->init("../../../TestViewer/toTest.import"), XML_OK);
			Assert::AreEqual(xmlManager->getInt("vpm"), 32);
		}

		TEST_METHOD(GetIntOK2)
		{
			Assert::AreEqual(xmlManager->init("../../../TestViewer/toTest.import"), XML_OK);
			Assert::AreEqual(xmlManager->getInt("x"), 320);
		}

		TEST_METHOD(GetIntKO1)
		{
			Assert::AreEqual(xmlManager->init("../../../TestViewer/toTest.import"), XML_OK);
			Assert::AreEqual(xmlManager->getInt("doesnotexist"), XML_GET_INT_ERROR);
		}

		TEST_METHOD(GetIntKO2)
		{
			Assert::AreEqual(xmlManager->init("../../../TestViewer/toTest.import"), XML_OK);
			Assert::AreEqual(xmlManager->getInt("path"), XML_GET_INT_ERROR);
		}


		/* EDIT STRING */

		TEST_METHOD(EditStringOK1)
		{
			Assert::AreEqual(xmlManager->init("../../../TestViewer/toTest.import"), XML_OK);
			Assert::IsTrue(xmlManager->edit("path", "success"));
		}

		TEST_METHOD(EditStringOK2)
		{
			Assert::AreEqual(xmlManager->init("../../../TestViewer/toTest.import"), XML_OK);
			Assert::IsTrue(xmlManager->edit("color", "success"));
		}

		TEST_METHOD(EditStringKO)
		{
			Assert::AreEqual(xmlManager->init("../../../TestViewer/toTest.import"), XML_OK);
			Assert::IsFalse(xmlManager->edit("doesnotexist", "fail"));
		}


		/* EDIT INT */

		TEST_METHOD(EditIntOK1)
		{
			Assert::AreEqual(xmlManager->init("../../../TestViewer/toTest.import"), XML_OK);
			Assert::IsTrue(xmlManager->edit("x", 1));
		}

		TEST_METHOD(EditIntOK2)
		{
			Assert::AreEqual(xmlManager->init("../../../TestViewer/toTest.import"), XML_OK);
			Assert::IsTrue(xmlManager->edit("path", 1));
		}

		TEST_METHOD(EditIntKO)
		{
			Assert::AreEqual(xmlManager->init("../../../TestViewer/toTest.import"), XML_OK);
			Assert::IsFalse(xmlManager->edit("doesnotexist", -1));
		}


		/* EDIT FLOAT */

		TEST_METHOD(EditFloatOK1)
		{
			Assert::AreEqual(xmlManager->init("../../../TestViewer/toTest.import"), XML_OK);
			Assert::IsTrue(xmlManager->edit("vpm", (float) 1.0));
		}

		TEST_METHOD(EditFloatOK2)
		{
			Assert::AreEqual(xmlManager->init("../../../TestViewer/toTest.import"), XML_OK);
			Assert::IsTrue(xmlManager->edit("path", (float) 1.0));
		}

		TEST_METHOD(EditFloatKO)
		{
			Assert::AreEqual(xmlManager->init("../../../TestViewer/toTest.import"), XML_OK);
			Assert::IsFalse(xmlManager->edit("doesnotexist", (float) -1.0));
		}


		/* SAVE */

		TEST_METHOD(SaveOK)
		{
			std::string newFilePath = "../../../TestViewer/toTestSaveOK.import";
			std::string newStr = "success";
			int newInt = 123;
			float newFloat = 123.0;

			Assert::AreEqual(xmlManager->init("../../../TestViewer/toTest.import"), XML_OK);
			Assert::IsTrue(xmlManager->edit("path", newStr));
			Assert::IsTrue(xmlManager->edit("count", newInt));
			Assert::IsTrue(xmlManager->edit("vpm", newFloat));

			Assert::IsTrue(xmlManager->save(newFilePath));

			Assert::AreEqual(xmlManager->init(newFilePath), XML_OK);
			
			std::string res = xmlManager->getString("path");
			Assert::IsTrue(res.compare(newStr) == 0);
			Assert::AreEqual(xmlManager->getInt("count"), newInt);
			Assert::AreEqual(xmlManager->getFloat("vpm"), newFloat);
		}

		TEST_METHOD(SaveKO1)
		{
			Assert::AreEqual(xmlManager->init("../../../TestViewer/toTest.import"), XML_OK);
			
			Assert::IsFalse(xmlManager->save(""));
			Assert::AreEqual(xmlManager->init(""), XML_IMPORT_ERROR);
		}

		TEST_METHOD(SaveKO2)
		{
			Assert::AreEqual(xmlManager->init("../../../TestViewer/toTest.import"), XML_OK);

			Assert::IsFalse(xmlManager->save("/"));
			Assert::AreEqual(xmlManager->init("/"), XML_IMPORT_ERROR);
		}

		TEST_METHOD(SaveKO3)
		{
			Assert::AreEqual(xmlManager->init("../../../TestViewer/toTest.import"), XML_OK);

			Assert::IsFalse(xmlManager->save("."));
			Assert::AreEqual(xmlManager->init("."), XML_IMPORT_ERROR);
		}
	};
}