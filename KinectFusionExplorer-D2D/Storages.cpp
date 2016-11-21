#include "IStorage.h"
#include "Storages.h"

IStorage * IStorage::factory(int selection) {
	switch (selection)
	{
		case 1:
			return new PLYStorage;
			break;
		default:
			break;
	}
}