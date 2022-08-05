#include <iostream>
#include <exception>

#include "system/system.hpp"


int main(int argc, char** argv)
{
	try
	{
		std::string mode;

		std::cout << "Possible modes: \n\t1. update-binance\n\t2. label\n\t3. label-study\n\t4. default" << std::endl;
		std::cout << "Mode: ";
		std::cin >> mode;

		if (mode == "update-binance")
		{
			sentiment::System().update_binance();
		}
		else if (mode == "label")
		{
			sentiment::System().label();
		}
		else if (mode == "label-study")
		{
			sentiment::System().label_study();
		}
		else
		{
			sentiment::System().run();
		}
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}

	system("pause");

	return EXIT_SUCCESS;
}