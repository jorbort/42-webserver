#include "../Includes/ConfigParser.hpp"
#include <fstream>
#include <stdexcept>
#include <string>

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
	void ConfigParser::setConfPath(std::string path)
	{
		this->configPath = path;
	}

	void ConfigParser::ParseConfig()
	{
		std::ifstream file;
		std::string line = "";
		bool server_start = false;

		file.open(this->getpath().c_str(), std::ios::in );
		if (file.is_open() == false)
			throw std::invalid_argument("");
		while (!file.eof())
		{
			getline(file, line);
			if (line[0] == '\n')
				continue;
			_ConfFile.push_back(line);
		}
		std::vector<std::string>::iterator start = _ConfFile.begin();
		while (start != _ConfFile.end())
		{
			std::cout << *start;
			start++;
		}
	}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */
