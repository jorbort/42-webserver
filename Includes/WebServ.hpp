#pragma once



#include <sys/socket.h>
//#include <sys/epoll.h>
#include <string>
#include "ConfigParser.hpp"
#include "Logger.hpp"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdexcept>
#include <string.h>
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>


class Server
{
	private:
		int sfd;
	public:
		Server(void);
		Server(std::string &path);
		Server( Server const & src );
		Server &operator=( Server const & rhs );
		~Server();

		ConfigParser conf;
		int makeSocket(sockaddr_in &port, int portlen);
		//void RunServer(void);
	class SocketException : std::exception
	{
		const char* what() const throw();
	};
};
