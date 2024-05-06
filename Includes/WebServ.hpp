#pragma once

#define RED  "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define CYAN "\033[106m"
#define RESET "\033[0m"

#include <sys/socket.h>
#include <sys/epoll.h>
#include <string>

class Server
{
	private:
		int sfd;
	public:
	void CreateSocket();
	static void ParseConfig(std::string &arg);
	static void ParseConfig(void);
	void RunServer(void);



	class SocketException : std::exception
	{
		const char* what() const throw();
	};
};
