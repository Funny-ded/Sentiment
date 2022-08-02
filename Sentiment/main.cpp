#include <iostream>
#include <exception>

#include "system/system.hpp"


int main(int argc, char** argv)
{
	try
	{
		sentiment::System().run();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}

	system("pause");

	return EXIT_SUCCESS;
}