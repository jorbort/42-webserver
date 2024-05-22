#include "../Includes/ServerConfigs.hpp"
#include <stdexcept>
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
	this->autoindex = false;
	this->ClientMaxBodySize = 300000000;
	this->index = "";
	this->_fd = 0;
	this->root = "";
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


// /*
// ** --------------------------------- OVERLOAD ---------------------------------
// */

// ServerConfigs &				ServerConfigs::operator=( ServerConfigs const & rhs )
// {
// 	//if ( this != &rhs )
// 	//{
// 		//this->_value = rhs.getValue();
// 	//}
// 	return *this;
// }

// std::ostream &			operator<<( std::ostream & o, ServerConfigs const & i )
// {
// 	//o << "Value = " << i.getValue();
// 	return o;
// }


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
	hostIp = htonl(ip);
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

/* ************************************************************************** */
