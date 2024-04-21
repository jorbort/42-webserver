#pragma once

#include <iostream>
#include "WebServ.hpp"


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