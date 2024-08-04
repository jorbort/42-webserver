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
		std::map<int, size_t> clientToServer;
	public:
		bool keepAlive;
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
		void acceptNewConnection(int fd, int epollFd, size_t serverIndex);
		ssize_t readClientData(int clientFd,char *&requestString);
		ssize_t readHeader(int clientFd, char *&requestString);
		ssize_t readBody(int clientFd, char *&requestString, std::string &tequestChecker, 
		size_t totalBytesRead);
		ssize_t	readChunkedBody(int clientFd, char *&requestString,
		size_t totalBytesRead);
		static void signalHandler(int signum);
		size_t getServerIndex(int fd);
	class SocketException : std::exception
	{
		const char* what() const throw();
	};
};
