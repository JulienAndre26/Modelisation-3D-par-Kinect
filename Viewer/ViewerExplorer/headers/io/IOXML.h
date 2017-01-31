#pragma once

#include <QtXml>
#include <unordered_map >
#include "IIO.h"

#define XML_IMPORT_ERROR 2
#define XML_OK 1

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
	int load(char * filename, void* objectToLoad);
	int save(char * filename, void* objectToSave);


};