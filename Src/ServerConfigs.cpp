#include "../Includes/ServerConfigs.hpp"
#include <stdexcept>
#define MAX_PORT 65535
#include <exception>

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
	this->serverName = "";
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


/* ************************************************************************** */
