#pragma once

#include <string>

class ICore {

public:
	virtual void* merge		(std::string* file_path_from, std::string* file_path_to) = 0;
	virtual int compress	(std::string* file_path) = 0;
	virtual void* other		(std::string* file_path) = 0;
};
