#pragma once

#ifndef IIO_H
#define IIO_H

/*
* Abstract class to 
*/
class IIO {

public:
	int load(char * filename, void* objectToLoad);
	int save(char * filename, void* objectToSave);
};

#endif IIO_H