#include "../Includes/Logger.hpp"

void Logger::print(std::string level, std::string message)
{
	std::string compare[3] = {"Error", "Warning", "Ok"};
	int i = 0;
	for(; i < 3 ; i++)
	{
		if (level == compare[i])
		{
			break;
		}
	}
	switch (i)
	{
		case 0 :
		{
			std::cerr << RED << message << std::endl;
			std::cerr << RESET;
			break;
		}
		case 1 :
		{
			std::cerr << YELLOW << message << std::endl;
			std::cerr << RESET;
			break;
		}
		case 2 :
		{
			std::cout << GREEN << message <<std::endl;
			std::cout << RESET;
			break;
		}
		default :
		{
			std::cerr << CYAN << "invalid logger level" << std::endl;
			std::cerr << RESET;
		}
	}
}
