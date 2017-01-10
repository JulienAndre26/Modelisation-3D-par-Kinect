#include "XML.h"

XML * XML::instance = new XML();


///*
//* Init variables like document and the root node.
//* (Used to read files).
//*/
//void  XML::init(std::string filename) {
//	XML::document.Clear();
//	XMLError eResult = XML::document.LoadFile("SavedData.xml");
//	XMLCheckReadResult(eResult);
//	XML::root = XML::document.FirstChild();
//}
//
///*
//* Init variables like document and the root node.
//* (Used to write files).
//*/
//void XML::init() {
//	XML::document.Clear();
//	XML::root = XML::document.NewElement("root");
//}
//
///*
//* Allows to add a STRING element, taking the
//* first parameter as tag name.
//*/
//void XML::add(std::string name, std::string value) {
//	XML::element = XML::document.NewElement(name.c_str());
//	XML::element->SetText(value.c_str());
//	XML::root->InsertEndChild(XML::element);
//}
//
///*
//* Allows to add a INT element, taking the
//* first parameter as tag name.
//*/
//void XML::add(std::string name, int value) {
//	XML::element = XML::document.NewElement(name.c_str());
//	XML::element->SetText(value);
//	XML::root->InsertEndChild(XML::element);
//}
//
///*
//* Allows to add a FLOAT element, taking the
//* first parameter as tag name.
//*/
//void XML::add(std::string name, float value) {
//	XML::element = XML::document.NewElement(name.c_str());
//	XML::element->SetText(value);
//	XML::root->InsertEndChild(XML::element);
//}
//
//std::string XML::get(std::string name) {
//	XMLElement * el = XML::root->FirstChildElement(name.c_str());
//	if (el == nullptr)
//		return NULL;
//	return el->GetText();
//}
//
//
///*
//* Finalize the document and store it
//* using the first parameter as filename.
//*/
//bool XML::save(std::string filename) {
//	// add the root to the document
//	XML::document.InsertFirstChild(XML::root);
//
//	// save it on the disk
//	XMLError eResult = XML::document.SaveFile(filename.c_str());
//	// check for errors
//	XMLCheckResult(eResult);
//
//	// result ok
//	return true;
//}