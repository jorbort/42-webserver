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
	this->_ConfFile = rhs._ConfFile;
	this->_ConfOptions  = rhs._ConfOptions;
	this->configPath = rhs.configPath;

	return *this;
}

/*
** --------------------------------- METHODS ----------------------------------
*/
	void ConfigParser::setConfPath(std::string path)
	{
		this->configPath = path;
	}

	std::string &ConfigParser::getpath(void)
	{
		return (this->configPath);
	} 

	void ConfigParser::ParseConfig()
	{
		std::ifstream file;
		std::string line = "";

		file.open(this->getpath().c_str(), std::ios::in );
		if (file.is_open() == false)
			throw std::invalid_argument("");
		while (!file.eof())
		{
			getline(file, line);
			if (line[0] != '\n')
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
