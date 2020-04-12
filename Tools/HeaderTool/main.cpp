#include <iostream>

#include "ProjectGenerator.h"

int main (int argc, char* argv[])
{
	if (argc < 3) {
		std::cout << "Error\n";

		return -1;
	}

	ProjectGenerator projectGenerator;

	projectGenerator.Generate (argv [1], argv [2]);

	return 0;
}
