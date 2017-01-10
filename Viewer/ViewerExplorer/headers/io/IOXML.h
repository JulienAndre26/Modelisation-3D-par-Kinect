#pragma once

#include <QtXml>
#include "IIO.h"

#define XML_IMPORT_ERROR 2
#define XML_OK 1

class IOXML : public IIO {
private:
	static IOXML * instance;

	QDomDocument * document;
	QFile * file;
	QDomElement *  element;
	QDomNode * node;

public:
	
	static IOXML * Instance();

	int init();
	int init(std::string filename);

	std::string get(std::string name);
	void add(std::string name, std::string value);
	void add(std::string name, int value);
	void add(std::string name, float value);
	bool save(std::string filename);

	// inherited method
	int load(char * filename, void* objectToLoad);
	int save(char * filename, void* objectToSave);


};