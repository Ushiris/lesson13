#include <iostream>
#include "MyApplication.h"

int main() 
{
	MyApplication app;

	try 
	{
		app.ran();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;

	return 0;
}

