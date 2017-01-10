#pragma once

/*
* Abstract class to 
*/
class IIO {

public:
	int load(const char * filename, void* objectToLoad);
	int save(const char * filename, void* objectToSave);
};