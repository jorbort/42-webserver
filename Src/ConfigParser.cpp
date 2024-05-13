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
		epurString(*start);
		std::cout << *start;
		start++;
	}
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
		if (str[i] != ' ' && str[i] != '\t')
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

// 	void	get_epured(char *str, char *res)
// {
// 		int	i;
// 		int	j;
// 		int	flag;

// 		i = 0;
// 		j = 0;
// 		flag = 0;
// 		while (str[i] == ' ' || str[i] == '\t')
// 			i++;
// 		while (str[i])
// 		{
// 			if (str[i] == ' ' || str[i] == '\t')
// 				flag = 1;
// 			if (str[i] != ' ' && str[i] != '\t')
// 			{
// 				if (flag)
// 					res[j++] = ' ';
// 				flag = 0;
// 				res[j++] = str[i];
// 			}
// 			i++;
// 		}
// 		res[j] = '\0';
// 		free(str);
// }

}
/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */
