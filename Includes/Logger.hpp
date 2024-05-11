#pragma once

#include <iostream>

#define RED  "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define CYAN "\033[106m"
#define RESET "\033[0m"


class Logger
{
	private:
		Logger();
		Logger(Logger &cpy);
		~Logger();
		Logger &operator=(Logger &rhs);
	public :
		static void print(std::string level, std::string message);
};
