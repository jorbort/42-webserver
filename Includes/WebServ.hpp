#pragma once



#include <sys/socket.h>
#include <sys/epoll.h>
#include <string>
#include "ConfigParser.hpp"
#include "Logger.hpp"

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
		void RunServer(void);
	class SocketException : std::exception
	{
		const char* what() const throw();
	};
};
