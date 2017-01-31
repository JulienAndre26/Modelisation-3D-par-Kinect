
#include "tinyxml2.h"
#include <string>

using namespace tinyxml2;

// macro to check for errors
#ifndef XMLCheckResult
	#define XMLCheckResult(a_eResult) if (a_eResult != XML_SUCCESS) { printf("Error: %i\n", a_eResult); return true; }
	// should return a_eResult;
#endif

#ifndef XMLCheckReadResult
	#define XMLCheckReadResult(a_eResult) if (a_eResult != XML_SUCCESS) { printf("Error: %i\n", a_eResult); }
#endif

class XML {
private:
	// constructor
	//XML(){}
	// singleton reference
	static XML * instance;

	// members
	tinyxml2::XMLDocument document;
	tinyxml2::XMLNode *root;
	tinyxml2::XMLElement * element;

public:
	// constructor
	XML() {}

	static XML * XML::Instance() {
		return instance;
	}

	/*
	* Init variables like document and the root node.
	* (Used to read files).
	*/
	void init(std::string filename) {
		document.Clear();
		XMLError eResult = document.LoadFile(filename.c_str());
		XMLCheckReadResult(eResult);
		root = document.FirstChild();
	}

	/*
	* Init variables like document and the root node.
	* (Used to write files).
	*/
	void init() {
		document.Clear();
		root = document.NewElement("root");
	}

	/*
	* Allows to add a STRING element, taking the
	* first parameter as tag name.
	*/
	bool add(std::string name, std::string value) {
		element = document.NewElement(name.c_str());
		element->SetText(value.c_str());
		return (root->InsertEndChild(element) != 0);
	}

	/*
	* Allows to add a INT element, taking the
	* first parameter as tag name.
	*/
	bool add(std::string name, int value) {
		if (value == NULL)
			return false;

		element = document.NewElement(name.c_str());
		element->SetText(value);
		return (root->InsertEndChild(element) != 0);
	}

	/*
	* Allows to add a FLOAT element, taking the
	* first parameter as tag name.
	*/
	bool add(std::string name, float value) {
		if (value == NULL)
			return false;

		element = document.NewElement(name.c_str());
		element->SetText(value);
		return (root->InsertEndChild(element) != 0);
	}

	/*
	* Retrieve an element according to its name
	* passed as first parameter.
	*/
	std::string get(std::string name) {
		XMLElement * el = root->FirstChildElement(name.c_str());
		if (el == nullptr)
			return std::string("ERROR");
		return el->GetText();
	}


	/*
	* Finalize the document and store it
	* using the first parameter as filename.
	*/
	bool save(std::string filename) {
		// add the root to the document
		document.InsertFirstChild(root);

		// save it on the disk
		XMLError eResult = document.SaveFile(filename.c_str());
		// check for errors
		XMLCheckResult(eResult);

		// result ok
		return true;
	}
};