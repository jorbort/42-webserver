#include "WebServ.hpp"
#include <stdexcept>
#include "HTTPRequest.hpp"
#include "HttpRequestParser.hpp"
#include "Response.hpp"


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
	while (i < this->conf.nOfServers)
	{
			this->conf._servers[i]->initSocket();
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

ssize_t Server::readHeader(int clientFd, char *&requestString)
{
    char buffer[1024];
	long bufferSize = 1024;
	ssize_t bytesRead;
	ssize_t totalBytesRead = 0;
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
		if (strstr(requestString, "\r\n\r\n") != NULL){
			break;
		}
    }
	if (bytesRead == -1){
		delete [] requestString;
		requestString = NULL;
		Logger::print("Error", "Error reading from client");
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
	return totalBytesRead;
}

ssize_t Server::readBody(int clientFd, char *&requestString, std::string &requestChecker)
{
	long bytesRead = 0;
	long contentLength = 0;
	size_t headerSize = requestChecker.find("\r\n\r\n") + 4;
	size_t contentLengthPos = requestChecker.find("Content-Length");
	size_t contentLengthEnd = requestChecker.find("\r\n", contentLengthPos);
	if (contentLengthPos != std::string::npos )
	{
		contentLength = std::stol(requestChecker.substr(contentLengthPos + 16, contentLengthEnd - contentLengthPos - 16));
	}
	else{
		return -1;
	}
	char buffer[contentLength];
	bytesRead = read(clientFd, buffer, contentLength);
	if (bytesRead == -1)
	{
		delete [] requestString;
		requestString = NULL;
		Logger::print("Error", "Error reading from client");
		return -1;
	}
	char *newBuffer = new char[headerSize + contentLength + 1];
	memcpy(newBuffer,requestString, headerSize + contentLength);
	memcpy(newBuffer + (headerSize + contentLength), buffer, bytesRead);
	delete[] requestString;
	requestString = newBuffer;
	requestString[headerSize + contentLength] = '\0';
	return bytesRead;
}

ssize_t Server::readClientData(int clientFd, char *&requestString) {
    long bufferSize = 1024;
	ssize_t bytesRead;
	ssize_t totalBytesRead = 0;
	std::string requestChecker = "";

	requestString = new char[bufferSize];
	requestString[0] = '\0';
	totalBytesRead = readHeader(clientFd, requestString);
    if (totalBytesRead == -1)
	{
		delete [] requestString;
		requestString = NULL;
		Logger::print("Error", "Error reading from client");
		return -1;
	}
	requestChecker = requestString;
	if (requestChecker.find("Content-Length") != std::string::npos)
	{
		int bodySize = 0;
		bodySize = readBody(clientFd, requestString, requestChecker);
		if ( bodySize == -1)
		{
			delete [] requestString;
			requestString = NULL;
			Logger::print("Error", "Error reading from client");
			return -1;
		}
		totalBytesRead += bodySize;
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
		std::cout << "waiting for connection" <<std::endl;
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
					if (request._ErrorCode != 0)
						std::cout << "ERROR: BAD REQUEST 1" << std::endl;
					//std::cout << request._ContentLength << std::endl;
					//std::cout << std::boolalpha << request._chunked << std::endl;
					Response response;
					std::cout << requestSize << std::endl;
					std::cout << request._ContentLength <<  std::endl;
					 
					std::string response_str = response.createResponse(request);
					write(events[n].data.fd, response_str.c_str(),response_str.size());
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
