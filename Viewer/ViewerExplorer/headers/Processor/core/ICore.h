#pragma once

class ICore {

public:
	virtual void* merge		(void* arg1, void* arg2) = 0;
	virtual void* compress	(void* arg) = 0;
	virtual void* other		(void* arg) = 0;
};
