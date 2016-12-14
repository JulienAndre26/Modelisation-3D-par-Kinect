#pragma once

#ifndef PCLCORE_H
#define PCLCORE_H

#include "ICore.h"
#include "custom_typedef.h"
#include "IOPLY.h"

class PCLCore : public ICore {
private:
	int someattribute;

public:
	void* merge(void* arg1, void* arg2);
	void* compress(void* arg);
	void* other(void* arg);

};

#endif