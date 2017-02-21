#pragma once

#include <QtXml>
#include <unordered_map>
#include "IIO.h"

#ifndef XML_IMPORT_ERROR
#define XML_IMPORT_ERROR 2
#endif // !XML_IMPORT_ERROR

#ifndef XML_OK
#define XML_OK 1
#endif // !XML_OK

#ifndef XML_GET_STR_ERROR
#define XML_GET_STR_ERROR "ERROR"
#endif // !XML_GET_STR_ERROR

#ifndef XML_GET_INT_ERROR
#define XML_GET_INT_ERROR -1
#endif // !XML_GET_INT_ERROR

#ifndef XML_GET_FLOAT_ERROR
#define XML_GET_FLOAT_ERROR M_PI
#endif // !XML_GET_FLOAT_ERROR


class IOXML : public IIO {
private:
	static IOXML * instance;

	QDomDocument * document;
	QFile * file;
	QDomElement *  element;
	QDomElement  root;
	QDomNode * node;

	QDomElement * findElement(std::string name);

public:
	
	static IOXML * Instance();

	int init();
	int init(std::string filename);

	template<typename T>
	T get(std::string name);

	std::string getString(std::string name);
	int getInt(std::string name);
	float getFloat(std::string name);
	std::unordered_map <std::string, float> * getMatrix();

	void add(std::string name, std::string value);
	void add(std::string name, int value);
	void add(std::string name, float value);

	bool edit(std::string name, std::string newValue);
	bool edit(std::string name, int newValue);
	bool edit(std::string name, float newValue);

	bool save(std::string filename);

	// inherited method
	int load(const char * filename, void* objectToLoad) { return XML_GET_INT_ERROR; }
	int save(const char * filename, void* objectToSave) { return XML_GET_INT_ERROR; }


};