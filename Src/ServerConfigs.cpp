#include "../Includes/ServerConfigs.hpp"
#include <stdexcept>
#include <vector>
#define MAX_PORT 65535
#include <exception>
#include <cstdlib>
#include <arpa/inet.h>

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

ServerConfigs::ServerConfigs()
{
	this->listen = 0;
	this->autoindex = -1;
	this->clientMaxBodySize = 300000000;
	this->index = "";
	this->_fd = 0;
	this->root = "";
	this->hostIp = 0;
	this->initErrorPages();
}

// ServerConfigs::ServerConfigs( const ServerConfigs & src )
// {
// }


// /*
// ** -------------------------------- DESTRUCTOR --------------------------------
// */

ServerConfigs::~ServerConfigs()
{
}

/*
** --------------------------------- METHODS ----------------------------------
*/
void ServerConfigs::initErrorPages()
{
	_errorPages[301] = "";
	_errorPages[302] = "";
	_errorPages[400] = "";
	_errorPages[401] = "";
	_errorPages[402] = "";
	_errorPages[403] = "";
	_errorPages[404] = "";
	_errorPages[405] = "";
	_errorPages[406] = "";
	_errorPages[500] = "";
	_errorPages[501] = "";
	_errorPages[502] = "";
	_errorPages[503] = "";
	_errorPages[505] = "";
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/

void ServerConfigs::setListen(const std::string &port)
{
	if (port.length() > 5)
		throw std::out_of_range("listen port must be number between 0 and 65535");
	int listen = std::atoi(port.c_str());
	if (listen < 0 || listen > MAX_PORT)
		throw std::out_of_range("listen port must be number between 0 and 65535");
	this->listen = listen;
}

void ServerConfigs::setHost(const std::string &host)
{
	int octet[4];
	size_t start = 0;
	size_t end;

	if (host.length() > 16 || host.length() < 7)
		throw std::invalid_argument("invalid ip format");
	for (int i = 0 ; i < 4; ++i)
	{
		end = host.find('.',start);
		if (end == std::string::npos)
			end = host.length();
		octet[i] = std::atoi(host.substr(start, end - start).c_str());
		start = end + 1;
	}
	for (int j = 0; j < 4; j++)
	{
		if (octet[j] > 255 || octet[j] < 0)
			throw std::invalid_argument("ip address must becomposed of numbers in ranges from 0 to 255");
	}
	u_int32_t ip = (octet[0] << 24) | (octet[1] << 16) | (octet[2] << 8) | octet[3];
	this->hostIp = htonl(ip);
}
void ServerConfigs::setName(const std::string &name)
{
	size_t begin = 1;
	size_t end;
	while(42)
	{
		end = name.find(" ", begin);
		if (end == std::string::npos)
		{
			this->serverName.push_back(name.substr(begin));
			break ;
		}
		this->serverName.push_back(name.substr(begin, end- begin));
		begin = end + 1;
		if (begin == std::string::npos)
			break;
	}
}

void ServerConfigs::addErrorPage(const std::string &errors)
{
	int key;
	std::string value = "";
	size_t begin = 0;
	size_t end;
	std::string tmp = "";
	while (42)
	{
		end = errors.find(" ", begin);
		tmp = errors.substr(begin, end - begin);
		if (tmp .length() > 3)
			throw std::invalid_argument("error codes ranges are int the 100s to 500s");
		key = std::atoi(tmp.c_str());
		if (key > 511)
			throw std::invalid_argument("the biggest error code is 511");
		begin = end + 1;
		end = errors.find(" ", begin);
		if (end == std::string::npos)
		{
			value = errors.substr(begin);
			this->_errorPages[key] = value;
			break;
		}
		value = errors.substr(begin, end - begin);
		this->_errorPages[key] = value;
		begin = end + 1;
	}
}

void ServerConfigs::setBodySize(const std::string &number)
{
	long num;
	int mult;
	if (number.length() > 10)
		throw std::invalid_argument("body size is to big max size is 10M bytes");
	bool isChar = number.find_first_of("MmgGkK") != std::string::npos ? true :false;
	if(isChar)
	{
		if (number[number.size() -1] == 'M' || number[number.size() -1] =='m' )
			mult = 1048576;
		if (number[number.size()-1] == 'G' || number[number.size() -1] == 'g')
			throw std::invalid_argument("body size is to big max size is 10M bytes");
		if (number[number.size()-1] == 'K' || number[number.size() -1] == 'k')
			mult =  1024;
		num = mult * std::atoi(number.substr(0, number.size()).c_str());
	}
	else
	{
		num = std::atoi(number.substr(0, number.size()).c_str());
	}
	this->clientMaxBodySize = num;
}

void ServerConfigs::setRoot(const std::string &root)
{
	if (root.size() == 0)
		throw std::invalid_argument("invalid root address");
	for(int i = 0; root[i]; i++)
	{
		if (!isalnum(root[i]) && root[i] != '-' && root[i] != '_' && root[i] != '.' && root[i] != '/')
			throw std::invalid_argument("forbidden character found in root address");
	}
	this->root = root;
}

void ServerConfigs::setIndex(const std::string &index)
{
	this->index = index;
}
void ServerConfigs::toggleAutoindex(const std::string &status)
{
	if (this->autoindex != -1)
		throw std::invalid_argument("Error repeated argument, autoindex");
	
	else
	{
		if (status == "on")
			this->autoindex = 1;
		else if (status == "off")
			this->autoindex = 0;
		else
			throw std::invalid_argument("invalid autoindex argument");
	}
}

void ServerConfigs::addLocation(const Location &location)
{
  this->locations.push_back(location);
}

int ServerConfigs::getListen(void) const
{
    return this->listen;
}

std::vector<Location> ServerConfigs::getLocations(void) const
{
    return this->locations;
}

std::vector<std::string> ServerConfigs::getServerName(void) const
{
    return this->serverName;
}

long ServerConfigs::getClientMaxBodySize(void) const
{
    return this->clientMaxBodySize;
}

std::string ServerConfigs::getIndex(void) const
{
    return this->index;
}

std::string ServerConfigs::getRoot(void) const
{
    return this->root;
}

struct sockaddr_in *ServerConfigs::getServerAddress(void) const
{
    return this->_serverAddress;
}
int ServerConfigs::getSocket(void) const
{
    return this->_fd;
}

int ServerConfigs::getAutoindex(void) const
{
    return this->autoindex;
}

const std::string &ServerConfigs::getErrorPage(int error) const
{
	static std::string nullstr; 
    const std::map<int,std::string>::const_iterator it = this->_errorPages.find(error);
    if (it == _errorPages.end())
		return (nullstr);
	return it->second;
}

unsigned int ServerConfigs::getHostIp() const
{
	return this->hostIp;
}

void ServerConfigs::checkServer(ServerConfigs &server)
{
	if (server.getHostIp() == 0)
		server.setHost("127.0.0.1");
	if (server.getListen() == 0)
		server.setListen("4242");
	if (server.getRoot().empty())
		server.setRoot("docs/web");
}

