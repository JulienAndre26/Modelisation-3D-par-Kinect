#pragma once

/*
* Abstract class to 
*/
class IIO {

public:
	int load(char * filename, void* objectToLoad);
	int save(char * filename, void* objectToSave);
};