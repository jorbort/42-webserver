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

ConfigParser &ConfigParser::operator=( ConfigParser const & rhs )
{
	this->_ConfFile = rhs._ConfFile;
	this->_ConfOptions  = rhs._ConfOptions;
	this->configPath = rhs.configPath;

	return *this;
}

/*
** --------------------------------- METHODS ----------------------------------
*/

void ConfigParser::findConfigs()
{
	std::string key;
	std::string value;
	std::string line;
	std::vector<std::string>::iterator vBegin = _ConfFile.begin() , vEnd = _ConfFile.end();
	bool bracket = false;
	int bracketCount = 0;

	for (; vBegin != vEnd; vBegin++)
	{
		line = *vBegin;
		if (line[line.length()] == '{')
		{
			bracket = true;
			bracketCount++;
		}
		if (!line.compare(0,6, "listen"))
		{
			_ConfOptions["listen"] = line.substr(line.find(" ") + 1, (line.length()));
		}
	}
	std::cout << _ConfOptions["listen"] << std::endl;
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
		if (line[0] != '\n' && !line.empty() && line[0] != '#')
		{
			epurString(line);
			_ConfFile.push_back(line);
		}
	}
	file.close();
	findConfigs();
}

void ConfigParser::epurString(std::string &str)
{
	std::string res = "";
	int i = 0;
	bool flag = false;

	while (str[i] == ' ' || str[i] == '\t')
		i++;
	while (str[i])
	{
		if (str[i] == ' ' || str[i] == '\t')
			flag = true;
		if (str[i] != ' ' && str[i] != '\t' && str[i] != ';')
		{
			if (flag)
				res.push_back(32);
			flag = 0;
			res.push_back(str[i]);
		}
		i++;
	}
	str = res;
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/

void ConfigParser::setConfPath(std::string &path)
{
	this->configPath = path;
}

std::string &ConfigParser::getpath(void)
{
	return (this->configPath);
}


/* ************************************************************************** */
