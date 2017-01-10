#include "IOXML.h"
#include <iostream>

IOXML * IOXML::instance = new IOXML();

IOXML *  IOXML::Instance() {
	return instance;
}

int IOXML::init() {
	return XML_IMPORT_ERROR;
}

int IOXML::init(std::string filename) {
	// open as a QDomDocument
	document = new QDomDocument("document");
	// open the file
	file = new QFile(filename.c_str());
	// check if its openable
	if (!file->open(QIODevice::ReadOnly))
	{
		return XML_IMPORT_ERROR;
	}
	// add content of the file to the document object
	if (!document->setContent(file))
	{
		file->close();
		return XML_IMPORT_ERROR;
	}
	element = &(document->firstChild().toElement());
	// everything is ok
	return XML_OK;
}

/*
* Method that returns the first occurence
* of an element according to its tag name.
*/
std::string IOXML::get(std::string name) {
	element = &(document->firstChild().toElement());

	// retrieve element list
	QDomNodeList  elt = element->elementsByTagName(QString::fromStdString(name));


	std::cout << name << " with size : " << elt.size() << std::endl;
	// if some elements have been found
	if (elt.size() > 0)
		// return the first one
		return  elt.at(0).toElement().text().toLocal8Bit().constData();
	else
		return std::string("ERROR");
}

void IOXML::add(std::string name, std::string value) {

}

void IOXML::add(std::string name, int value) {

}

void IOXML::add(std::string name, float value) {

}

bool IOXML::save(std::string filename) {
	return false;
}