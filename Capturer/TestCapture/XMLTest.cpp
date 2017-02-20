//#include "stdafx.h"
#include "CppUnitTest.h"

#include "../CaptureExplorer/XML.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TestCapture
{		
	TEST_CLASS(XMLTest)
	{
	private:
		XML * xmlManager;

	public:
		TEST_METHOD_INITIALIZE(TestInit)
		{
			xmlManager = new XML();
			xmlManager->init();
		}

		TEST_METHOD(XmlAddOK1)
		{
			Assert::IsTrue(xmlManager->add("test", "testOK"));
		}

		TEST_METHOD(XmlAddOK2)
		{
			Assert::IsTrue(xmlManager->add("test", 5));
		}

		TEST_METHOD(XmlAddOK3)
		{
			Assert::IsTrue(xmlManager->add("test", (float) .5));
		}

		TEST_METHOD(XmlAddKO1)
		{
			Assert::IsFalse(xmlManager->add("test", NULL));
		}

		TEST_METHOD(XmlGetOK1)
		{
			std::string field = "string";
			std::string value = "STRING";
			std::string filename = "./xml.import";

			xmlManager->add(field, value);
			xmlManager->save(filename);

			xmlManager->init(filename);
			std::string res = xmlManager->get(field);
			Assert::IsTrue(value.compare(res) == 0);
		}

		TEST_METHOD(XmlGetOK2)
		{
			std::string field = "integer";
			int value = 1;
			std::string filename = "./xml.import";

			xmlManager->add(field, value);
			xmlManager->save(filename);

			xmlManager->init(filename);
			std::string res = xmlManager->get(field);
			Assert::IsTrue(std::to_string(value).compare(res) == 0);
		}

		TEST_METHOD(XmlGetOK3)
		{
			std::string field = "float";
			float value = 0.1;
			std::string filename = "./xml.import";

			xmlManager->add(field, value);
			xmlManager->save(filename);

			xmlManager->init(filename);
			std::string res = xmlManager->get(field);
			float tmp = stof(res);
			Assert::IsTrue(tmp == value);
		}

		TEST_METHOD(XmlGetKO)
		{
			std::string filename = "./xml.import";
			std::string err = "ERROR";

			xmlManager->init(filename);
			std::string res = xmlManager->get("DOES NOT EXIST");
			Assert::IsTrue(err.compare(res) == 0);
		}
	};
}