#include "../Includes/WebServ.hpp"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#define MAX_EVENTS 10000

void Server::RunServer(void)
{
	int new_socket;
	int epoll_fd = epoll_create1(0);
	int eventCount;

	std::string response = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";
	this->sfd = socket(AF_INET, SOCK_STREAM, 0);
	if (this->sfd == -1)
		throw SocketException();
	if (epoll_fd < 0)
		throw SocketException();
	sockaddr_in port;

	struct epoll_event event, events[MAX_EVENTS];
	int portlen = sizeof(port);

	event.events = EPOLLIN;
	event.data.fd = this->sfd;

	port.sin_family = AF_INET;
	port.sin_port = htons(4242);
	port.sin_addr.s_addr = INADDR_ANY;

	memset(port.sin_zero, '\0', sizeof(port.sin_zero));

	if (bind(this->sfd, (struct sockaddr *) &port, sizeof(port)) == -1)
		throw SocketException();

	if (listen(this->sfd, 10) < 0)
		throw SocketException();
	fcntl(this->sfd, F_SETFL, O_NONBLOCK);
	
	std::cout << "waiting for conection" << std::endl;
	while (42)
	{
		char buffer [30000] = {0};

		if (( new_socket = accept(this->sfd,(struct sockaddr *) &port, (socklen_t *)&portlen)) < 0)
		{
			if(errno == EAGAIN || errno == EWOULDBLOCK)
			{
					continue;
			}
			else
			{
				close(epoll_fd);
				throw SocketException();
			}
		}
		event.data.fd = new_socket;
		std::cout << new_socket << std::endl;
		if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, new_socket, &event))
		{
			close(epoll_fd);
			throw SocketException();
		}
		std::cout << "pasa epollctl" <<std::endl;
		eventCount = epoll_wait(epoll_fd, events, MAX_EVENTS, 30000);
		std::cout << eventCount;
		for(int i = 0; i < eventCount; i++)
		{
			if (read(events[i].data.fd, buffer, 30000) < 0)
				throw SocketException();
			std::cout << buffer << std::endl;
			write(new_socket, response.c_str(), response.length());

		}
		// close(new_socket);
	}
	close(epoll_fd);
}

const char * Server::SocketException::what() const throw()
{
	return ("error creating or binding the socket");
}
