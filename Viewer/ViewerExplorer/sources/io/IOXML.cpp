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
	root = document->documentElement();
	element = &(document->firstChild().toElement());
	// everything is ok
	return XML_OK;
}

QDomElement * IOXML::findElement(std::string name) {
	return &(root.firstChildElement(QString(name.c_str())));
}

/*
* Method that returns the first occurence
* of an element according to its tag name.
*/
std::string IOXML::get(std::string name) {
	QDomElement * elt = findElement(name);
	if (!elt->isNull()) 
		return elt->text().toLocal8Bit().constData();
	else
		return std::string("ERROR");
}

void IOXML::add(std::string name, std::string value) {

}

void IOXML::add(std::string name, int value) {

}

void IOXML::add(std::string name, float value) {

}

bool IOXML::edit(std::string name, std::string newValue) {
	QDomElement elt = root.firstChildElement(QString(name.c_str()));
	if (!elt.isNull() && !elt.firstChild().isNull()) {
		elt.firstChild().setNodeValue(newValue.c_str());
		return true;
	}
	return false;
}

bool IOXML::edit(std::string name, int newValue) {
	QDomElement elt = root.firstChildElement(QString(name.c_str()));
	if (!elt.isNull() && !elt.firstChild().isNull()) {
		elt.firstChild().setNodeValue(std::to_string(newValue).c_str());
		return true;
	}
	return false;
}

bool IOXML::edit(std::string name, float newValue) {
	QDomElement elt = root.firstChildElement(QString(name.c_str()));
	if (!elt.isNull() && !elt.firstChild().isNull()) {
		elt.firstChild().setNodeValue(std::to_string(newValue).c_str());
		return true;
	}
	return false;
}

bool IOXML::save(std::string filename) {
	try {
		file = new QFile(filename.c_str());

		if (file->open(QIODevice::WriteOnly)) {
			file->write(document->toString().toLocal8Bit().constData());
			file->close();
			return true;
		}
		
	}
	catch (const std::exception & e){
		std::cerr << e.what();
	}
	
	return false;
}