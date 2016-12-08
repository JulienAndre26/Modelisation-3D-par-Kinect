//#include "stdafx.h"


#pragma warning( disable : 4996 )
#include <iostream>
#include <string>
#include "ProcessorPCL.h"

using namespace std;

int main(int argc, char** argv) {
	
	string path;
	ProcessorPCL * processor = new ProcessorPCL();
	processor->merge(path);
	return EXIT_SUCCESS;
}
