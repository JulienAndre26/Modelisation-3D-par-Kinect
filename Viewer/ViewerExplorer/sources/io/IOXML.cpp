#include "IOXML.h"

#include <iostream>
#include <stdexcept>

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
template<typename T>
T IOXML::get(std::string name) {
	QDomElement * elt = findElement(name);
	if (!elt->isNull()) 
		return dynamic_cast<T>(elt->text().toLocal8Bit().constData());
	else
		return std::string(XML_GET_STR_ERROR);
}

std::string IOXML::getString(std::string name) {
	QDomElement * elt = findElement(name);
	if (!elt->isNull())
		return (std::string)(elt->text().toLocal8Bit().constData());
	else
		return std::string(XML_GET_STR_ERROR);
}

float IOXML::getFloat(std::string name) {
	QDomElement * elt = findElement(name);
	if (!elt->isNull())
	{	
		try {
			return std::stof(elt->text().toLocal8Bit().constData());
		} catch (std::invalid_argument& e)
		{
			std::cerr << e.what();
			return XML_GET_FLOAT_ERROR;
		}
	}
	else
		return XML_GET_FLOAT_ERROR;
}

int IOXML::getInt(std::string name) {
	QDomElement * elt = findElement(name);
	if (!elt->isNull())
	{
		try {
			return std::stoi(elt->text().toLocal8Bit().constData());
		}
		catch (std::invalid_argument& e)
		{
			std::cerr << e.what();
			return -1;
		}
	}
	else
		return -1;
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

std::unordered_map <std::string, float> * IOXML::getMatrix() {
	std::unordered_map <std::string, float> *result = new std::unordered_map <std::string, float>();
	(*result)[std::string("m11")] = getFloat(std::string("m11"));
	(*result)[std::string("m12")] = getFloat(std::string("m12"));
	(*result)[std::string("m13")] = getFloat(std::string("m13"));
	(*result)[std::string("m14")] = getFloat(std::string("m14"));
	
	(*result)[std::string("m21")] = getFloat(std::string("m21"));
	(*result)[std::string("m22")] = getFloat(std::string("m22"));
	(*result)[std::string("m23")] = getFloat(std::string("m23"));
	(*result)[std::string("m24")] = getFloat(std::string("m24"));

	(*result)[std::string("m31")] = getFloat(std::string("m31"));
	(*result)[std::string("m32")] = getFloat(std::string("m32"));
	(*result)[std::string("m33")] = getFloat(std::string("m33"));
	(*result)[std::string("m34")] = getFloat(std::string("m34"));

	(*result)[std::string("m41")] = getFloat(std::string("m41"));
	(*result)[std::string("m42")] = getFloat(std::string("m42"));
	(*result)[std::string("m43")] = getFloat(std::string("m43"));
	(*result)[std::string("m44")] = getFloat(std::string("m44"));

	return result;
}