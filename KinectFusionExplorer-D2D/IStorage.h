#pragma once


class IStorage {
private:

public:
	// standard destructor needed with some compilers
	virtual ~IStorage() {}

	// static factory
	static IStorage * factory(int selection);

	// save some content in some file
	virtual int save() = 0;

	// load some content from a file
	virtual int load() = 0;
};