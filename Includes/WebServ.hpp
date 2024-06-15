#pragma once



#include <sys/socket.h>
#include <sys/epoll.h>
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
#include <signal.h>



class Server
{
	private:
		int sfd;
	public:
		Server(void);
		Server(char *path);
		Server( Server const & src );
		Server &operator=( Server const & rhs );
		~Server();

		char *requestString;
		ConfigParser conf;
		int makeSocket(sockaddr_in &port, int portlen);
		void RunServer(void);
		void initCluster(void);
		bool isServerSocket(int fd);
		void acceptNewConnection(int fd, int epollFd);
		ssize_t readClientData(int clientFd,char *&requestString);
		static void signalHandler(int signum);
	class SocketException : std::exception
	{
		const char* what() const throw();
	};
};
