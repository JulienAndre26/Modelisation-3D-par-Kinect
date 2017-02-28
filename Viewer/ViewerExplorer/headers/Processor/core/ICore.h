#pragma once

#include <string>

/*
Interface to access the core, to merge meshes for instance.
*/

class ICore {

public:
	virtual void  merge		(const std::string& from, const std::string& to, const std::string& into) = 0;
	virtual int compress	(std::string* file_path) = 0;
	virtual void* other		(std::string* file_path) = 0;
};
