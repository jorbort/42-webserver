#include "../Includes/WebServ.hpp"
#include <stdexcept>


#define MAX_EVENTS 10000

Server::Server(void)
{

}

Server::Server(std::string &path)
{
    conf.setConfPath(path);
	try
	{
		conf.ParseConfig();
	}
	catch (std::exception &e)
	{
		Logger::print("Error", e.what());
		throw std::invalid_argument(" ");
	}
}
Server::~Server()
{

}

void Server::initCluster(void)
{
	size_t i = 0;
	while (i < this->conf.nOfServers)
	{
			this->conf._servers[i].initSocket();
			i++;
	}
}
void Server::RunServer(void)
{
	int newSocket;
	int eventCount;
	int epollFd = epoll_create1(0);
	if (epollFd < 0)
		throw SocketException();
	struct epoll_event event, events[MAX_EVENTS];
	event.events = EPOLLIN;
	for (size_t i = 0; i < conf.nOfServers;i++)
	{
		event.data.fd = conf._servers[i].getSocket();
		if (epoll_ctl(epollFd, EPOLL_CTL_ADD, conf._servers[i].getSocket(), &event) == -1)
			throw std::runtime_error("epoll_ctl failed to add fd");
	}
	Logger::printTrain();
	while(42)
	{
		int nfds = epoll_wait(epollFd, events, MAX_EVENTS, -1);
		if (nfds == -1)
			throw std::runtime_error("Error: epoll_wait failed");
		for(int n = 0 ; n < nfds; ++n)
		{
			//faltaimplementar esta parte correctamente
			//if (events[n].data.fd == conf)
		}
	}
}


// void Server::RunServer(void)
// {
// 	int new_socket;
// 	int epoll_fd = epoll_create1(0);
// 	int eventCount;
// 	std::string response = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";
// 	if (epoll_fd < 0)
// 		throw SocketException();
// 	struct epoll_event event, events[MAX_EVENTS];
// 	event.events = EPOLLIN;
// 	event.data.fd = this->sfd;

// 	while (42)
// 	{
// 		char buffer [30000] = {0};

// 		Logger::print("Ok", "waiting for conection...");
// 		if (( new_socket = accept(this->sfd,(struct sockaddr *) &port, (socklen_t *)&portlen)) < 0)
// 		{
// 				close(epoll_fd);
// 				throw SocketException();
// 		}
// 		event.data.fd = new_socket;
// 		if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, new_socket, &event))
// 		{
// 			close(epoll_fd);
// 			throw SocketException();
// 		}
// 		eventCount = epoll_wait(epoll_fd, events, MAX_EVENTS, 30000);
// 		for(int i = 0; i < eventCount; i++)
// 		{
// 			if (read(events[i].data.fd, buffer, 30000) < 0)
// 				throw SocketException();
// 			std::cout << buffer << std::endl;
// 			write(new_socket, response.c_str(), response.length());

// 		}
// 	}
// 	close(epoll_fd);
// }

Server::Server( const Server & src )
{
	*this = src;
}
Server &Server::operator=( Server const & rhs )
{
	this->conf = rhs.conf;
	this->sfd = rhs.sfd;
	return (*this);
}

const char * Server::SocketException::what() const throw()
{
	return ("error creating or binding the socket");
}
