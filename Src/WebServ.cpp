#include "../Includes/WebServ.hpp"
#include <stdexcept>
#include "../HttpRequest/HTTPRequest.hpp"
#include "../HttpRequest/HttpRequestParser.hpp"


#define MAX_EVENTS 10000

static Server *serverInstance = NULL;

Server::Server(void)
{
	std::string path = "config/default.conf";
	serverInstance = this;
	signal(SIGINT, Server::signalHandler);
	requestString = NULL;

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

void Server::signalHandler(int signum){
	if (serverInstance != NULL)
	{
		for (size_t i = 0; i < serverInstance->conf.nOfServers; i++)
		{
			close(serverInstance->conf._servers[i]->getSocket());
		}
		if (serverInstance->requestString != NULL)
			delete[] serverInstance->requestString;
		delete serverInstance;
		serverInstance = NULL;
		
	}
	exit(signum);

}

Server::Server(char *path)
{
	serverInstance = this;
	signal(SIGINT, Server::signalHandler);
	requestString = NULL;

	std::string arg = path;
    conf.setConfPath(arg);
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
	std::cout << this->conf.nOfServers << std::endl;
	while (i < this->conf.nOfServers)
	{
			this->conf._servers[i]->initSocket();
			std::cout << i << std::endl;
			i++;
	}
}

bool Server::isServerSocket(int fd)
{
    for (size_t i = 0; i < conf.nOfServers; i++)
    {
        if (conf._servers[i]->getSocket() == fd)
            return true;
    }
    return false;
}

void Server::acceptNewConnection(int fd,int epolFd)
{
	struct sockaddr_in clientAddr;
	socklen_t clientAddrLen = sizeof(clientAddr);
	int clientFd = accept(fd,(struct sockaddr *)&clientAddr,&clientAddrLen);
	if (clientFd == -1)
		throw std::runtime_error("Error: accept failed");
	if (fcntl(clientFd, F_SETFL, O_NONBLOCK) == -1)
		throw std::runtime_error("Error: fcntl failed");
	struct epoll_event event;
	event.events = EPOLLIN;
	event.data.fd = clientFd;
	if (epoll_ctl(epolFd, EPOLL_CTL_ADD, clientFd, &event) == -1)
		throw std::runtime_error("Error: epoll_ctl failed to add fd"); 	
}

ssize_t Server::readClientData(int clientFd, char *&requestString) {
    char buffer[1024];
	long bufferSize = 1024;
    ssize_t bytesRead;
	ssize_t totalBytesRead = 0;

	requestString = new char[bufferSize];
	requestString[0] = '\0';

    while ((bytesRead = read(clientFd, buffer, sizeof(buffer))) > 0) {
    	if (totalBytesRead + bytesRead > bufferSize){
	  		bufferSize *= 2;
	  		char *newBuffer = new char[bufferSize];
	  		memcpy(newBuffer,requestString,totalBytesRead);
	  		delete[] requestString;
	  		requestString = newBuffer;
		}
		memcpy(requestString + totalBytesRead, buffer, bytesRead);
		totalBytesRead += bytesRead;
		if (totalBytesRead > 4 && memcmp(requestString + totalBytesRead - 4, "\r\n\r\n", 4) == 0){
			break;
		}
    }
	if (bytesRead == -1){
		delete [] requestString;
		requestString = NULL;
		return bytesRead;
	}
	if (totalBytesRead < bufferSize){
		requestString[totalBytesRead] = '\0';
	}else{
		char *newBuffer = new char[bufferSize + 1];
		memcpy(newBuffer,requestString,totalBytesRead);
		newBuffer[totalBytesRead] = '\0';
		delete[] requestString;
		requestString = newBuffer;
	}
	this->requestString = requestString;
    return totalBytesRead;
}

void Server::RunServer(void)
{
	int epollFd = epoll_create1(0);
	if (epollFd < 0)
		throw SocketException();
	struct epoll_event event, events[MAX_EVENTS];
	event.events = EPOLLIN;
	for (size_t i = 0; i < conf.nOfServers;i++)
	{
		event.data.fd = conf._servers[i]->getSocket();
		if (epoll_ctl(epollFd, EPOLL_CTL_ADD, conf._servers[i]->getSocket(), &event) == -1)
			throw std::runtime_error("epoll_ctl failed to add fd");
	}
	Logger::printTrain();
	while(42)
	{
		std::cout << "witing for connection" <<std::endl;
		int nfds = epoll_wait(epollFd, events, MAX_EVENTS, 300000);
		if (nfds == -1)
			throw std::runtime_error("Error: epoll_wait failed");
		for(int n = 0 ; n < nfds; ++n)
		{
			if (isServerSocket(events[n].data.fd))
			{
				acceptNewConnection(events[n].data.fd, epollFd);
			}
			else
			{
				char * requestString = NULL;
				ssize_t requestSize = readClientData(events[n].data.fd, requestString);
				if (requestSize == 0)
				{
					close(events[n].data.fd);
					epoll_ctl(epollFd, EPOLL_CTL_DEL, events[n].data.fd, NULL);
				}else if (requestSize == -1)
				{
					close(events[n].data.fd);
					epoll_ctl(epollFd, EPOLL_CTL_DEL, events[n].data.fd, NULL);
				}else{
					//hettpRequestParser va aqui 
					HttpRequest request;
					HttpRequestParser Request_parser;
					Request_parser.parseRequest(request, requestString, requestSize);
					std::cout << request << std::endl;
					if (request._ErrorCode != 0)
						std::cout << "ERROR: BAD REQUEST" << std::endl;
					std::cout << request._ContentLength << std::endl;
					std::cout << std::boolalpha << request._chunked << std::endl;
					
				}
				delete[] requestString;
				requestString = NULL;
				this->requestString = NULL;
			}
		}
	}
	for (size_t i = 0; i < conf.nOfServers; i++)
	{
		close(conf._servers[i]->getSocket());
	}
	close(epollFd);
}

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
