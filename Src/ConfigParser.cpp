#include "../Includes/ConfigParser.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

ConfigParser::ConfigParser()
{
}

ConfigParser::ConfigParser( const ConfigParser & src )
{
	*this = src;
}


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

ConfigParser::~ConfigParser()
{
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

ConfigParser &				ConfigParser::operator=( ConfigParser const & rhs )
{
	if ( this != &rhs )
	{
		*this = rhs;
	}
	return *this;
}

/*
** --------------------------------- METHODS ----------------------------------
*/


/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */