#include "../Includes/WebServ.hpp"
#include <stdexcept>
#include "../HttpRequest/HTTPRequest.hpp"
#include "../HttpRequest/HttpRequestParser.hpp"
#include "../HttpResponse/Response.hpp"


#define MAX_EVENTS 10000

static Server *serverInstance = NULL;

std::string getALine(int clientFd)
{
	char c;
	std::string line;
	while(read(clientFd,&c,1) > 0)
	{
		if (c == '\n')
			break;
		if (c != '\r')
			line += c;
	}
	return line;
}

long htol(std::string hex)
{
	long num;
	std::istringstream(hex) >> std::hex >>  num;
	return num;
}

long ftStrtol(const std::string &str)
{
	long ret = 0;
	for (size_t i = 0; i < str.size(); i++)
	{
		if (str[i] < '0' || str[i] > '9')
			throw std::invalid_argument(" ");
		ret = ret * 10 + (str[i] - '0');
	}
	return ret;

}

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
	signal(SIGTSTP, Server::signalHandler);
	signal(SIGTERM, Server::signalHandler);
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

void Server::acceptNewConnection(int fd,int epolFd, size_t serverIndex)
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
	clientToServer[clientFd] = serverIndex;
}

ssize_t Server::readHeader(int clientFd, char *&requestString)
{
    char buffer[200];
	long bufferSize = 199;
	ssize_t bytesRead;
	ssize_t totalBytesRead = 0;
	while ((bytesRead = read(clientFd, buffer, sizeof(buffer))) > 0) {
    	if (totalBytesRead + bytesRead > bufferSize){
	  		bufferSize *= 2;
	  		char *newBuffer = new char[bufferSize + 1];
	  		memcpy(newBuffer,requestString,totalBytesRead);
	  		delete[] requestString;
	  		requestString = newBuffer;
		}
		memcpy(requestString + totalBytesRead, buffer, bytesRead);
		totalBytesRead += bytesRead;
		requestString[totalBytesRead] = '\0';
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

ssize_t Server::readBody(int clientFd, char *&requestString, std::string &requestChecker, size_t totalBytesRead)
{
	long bytesRead = 0;
	long contentLength = 0;
	size_t headerSize = requestChecker.find("\r\n\r\n") + 4;
	size_t contentLengthPos = requestChecker.find("Content-Length");
	size_t contentLengthEnd = requestChecker.find("\r\n", contentLengthPos);
	if (contentLengthPos != std::string::npos )
	{
		contentLength = ftStrtol(requestChecker.substr(contentLengthPos + 16, contentLengthEnd - contentLengthPos - 16));
	}
	else{
		return -1;
	}
	std::vector<char> buffer(contentLength);
	if (totalBytesRead == headerSize + contentLength)
	{	return (ssize_t)contentLength;}
    bytesRead = read(clientFd, buffer.data(), contentLength);
    if (bytesRead <= 0)
    {
        delete[] requestString;
        requestString = NULL;
        Logger::print("Error", "Error reading from client");
        return -1;
    }

    char *newBuffer = new char[headerSize + contentLength + 1];
    memcpy(newBuffer, requestString, totalBytesRead);
    memcpy(newBuffer + totalBytesRead, buffer.data(), bytesRead);

	delete[] requestString;
	requestString = newBuffer;
	requestString[headerSize + bytesRead] = '\0';
	return bytesRead;
}

ssize_t Server::readChunkedBody(int clientFd, char *&requestString,size_t totalByteRead)
{
	size_t chunkSize = 1;
	std::vector<char> buffer;
	long bytesRead;
	while(true)
	{
		chunkSize = htol(getALine(clientFd));
		if (chunkSize == 0)
			break;
		buffer.resize(chunkSize);
		bytesRead =  read(clientFd, buffer.data(), chunkSize);
		if (bytesRead == -1)
		{
			delete [] requestString;
			requestString = NULL;
			Logger::print("Error", "Error reading from cleint");
			return (-1);
		}
        char *tmpRequest = new char[totalByteRead + bytesRead + 1];
        if (requestString) {
            memcpy(tmpRequest, requestString, totalByteRead);
            delete[] requestString;
        }
        memcpy(tmpRequest + totalByteRead, buffer.data(), bytesRead);
        totalByteRead += bytesRead;
        tmpRequest[totalByteRead] = '\0';
        requestString = tmpRequest;
        getALine(clientFd);
    }
	std::string line;
    while (!(line = getALine(clientFd)).empty()) {
    }
    return totalByteRead;
}

ssize_t Server::readClientData(int clientFd, char *&requestString) {
    long bufferSize = 280;
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
	if (requestChecker.find("Content-Length") != std::string::npos && requestChecker.find("Transfer-Encoding: chunked") == std::string::npos)
	{
		int bodySize = 0;
		bodySize = readBody(clientFd, requestString, requestChecker, totalBytesRead);
		if ( bodySize == -1)
		{
			delete [] requestString;
			requestString = NULL;
			Logger::print("Error", "Error reading from client");
			return -1;
		}
		totalBytesRead += bodySize;
	}
	else if (requestChecker.find("Transfer-Encoding: chunked") != std::string::npos)
	{
		int bodySize = 0;
		bodySize = readChunkedBody(clientFd, requestString, totalBytesRead);
		if (bodySize == -1)
		{
			delete [] requestString;
			requestString = NULL;
			Logger::print("Error", "Error reading fomr client");
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
				size_t serverIndex = 0;
				for(size_t j = 0; j < conf.nOfServers; j++)
				{
					if(conf._servers[j]->getSocket() == events[n].data.fd)
					{
						serverIndex = j;
						break;
					}
				}
				acceptNewConnection(events[n].data.fd, epollFd, serverIndex);
			}
			else
			{
				char * requestString = NULL;
				ssize_t requestSize = readClientData(events[n].data.fd, requestString);
				if (requestSize == 0)
				{
					close(events[n].data.fd);
					epoll_ctl(epollFd, EPOLL_CTL_DEL, events[n].data.fd, NULL);
					clientToServer.erase(events[n].data.fd);
				}else if (requestSize == -1)
				{
					close(events[n].data.fd);
					epoll_ctl(epollFd, EPOLL_CTL_DEL, events[n].data.fd, NULL);
					clientToServer.erase(events[n].data.fd);
				}else{
					HttpRequest request;
					HttpRequestParser Request_parser;
					Request_parser.parseRequest(request, requestString, requestSize);
					std::cout << requestString <<std::endl;
					size_t serverIndex = getServerIndex(events[n].data.fd);
					Response response(request, conf._servers[serverIndex]);
					std::string response_str = response.createResponse();
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

size_t Server::getServerIndex(int fd){
	std::map<int, size_t>::iterator it = clientToServer.find(fd);
	if (it != clientToServer.end())
		return it->second;
	throw std::runtime_error("Error: client not found in map");
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
