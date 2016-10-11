
#include <reconstructmesdk/reme.h>
#include <iostream>

int main() {
	reme_context_t c;
	reme_context_create(&c);

	const char *version;
	int length;
	reme_context_get_version(c, &version, &length);

	std::cout << "Running ReconstructMe SDK version " << version << std::endl;

	reme_context_destroy(&c);
}