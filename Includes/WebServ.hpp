#pragma once

#define RED  "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define CYAN "\033[106m"
#define RESET "\033[0m"

#include <sys/socket.h>
#include <sys/epoll.h>
#include <string>
#include "ConfigParser.hpp"

class Server
{
	private:
		int sfd;
	public:
		void RunServer(void);
		ConfigParser conf;
	class SocketException : std::exception
	{
		const char* what() const throw();
	};
};
