#include "Core/Application.h"

#include <cstdlib>
#include <iostream>

int main()
{
	Application app;

	try
	{
		app.run();
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}