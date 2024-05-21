#include "../Includes/WebServ.hpp"


#define MAX_EVENTS 10000

Server::Server(void)
{
	//this->conf.defaultConf();
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
		// throw std::invalid_argument("can't open config file");
		std::cout<< e.what() << std::endl;
	}
}
Server::~Server()
{

}

// int Server::makeSocket(sockaddr_in &port, int portlen)
// {
// 	int n =  socket(AF_INET, SOCK_STREAM, 0);
// 	port.sin_family = AF_INET;
// 	port.sin_port = htons(std::atoi(conf.getValue("listen").c_str()));
// 	port.sin_addr.s_addr = INADDR_ANY;
// 	memset(port.sin_zero, '\0', sizeof(port.sin_zero));
// 	if (bind(n, (struct sockaddr *) &port, portlen) == -1)
// 		return -1;
// 	if (listen(n, 10) < 0)
// 		return -1;
// 	return (n);
// }

// void Server::RunServer(void)
// {
// 	int new_socket;
// 	int epoll_fd = epoll_create1(0);
// 	int eventCount;
// 	sockaddr_in port;
// 	int portlen = sizeof(port);

// 	std::string response = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";
// 	this->sfd = makeSocket(port, portlen);
// 	if (this->sfd == -1)
// 		throw SocketException();
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
